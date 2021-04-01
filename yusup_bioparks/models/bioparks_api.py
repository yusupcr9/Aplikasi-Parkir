import logging
# import base64
from odoo import api, fields, models, _
from odoo.addons.web.controllers.main import _serialize_exception
from datetime import datetime, timedelta
ADMIN_USER = 1

_logger = logging.getLogger(__name__)


class KendaraanAPI(models.Model):
    _inherit = 'bioparks.kendaraan'

    def api_get_kendaraan(self, no_mesin=False, offset=0, limit=None):
        read_list = [
            'name',
            'partner_id',
            'no_mesin',
            'sidik_jari_ids',
        ]
        if no_mesin:
            cek_keluar = self.env['bioparks.riwayat'].search([
                ('jenis', '=', 'inout'),
                ('no_mesin', '=', no_mesin),
                ('jam_masuk', '!=', False),
                ('jam_keluar', '!=', False)
            ], order="id desc", limit=1)
            if cek_keluar:
                return ([1, 2], 'Anda Harus Masuk Dulu!!!')
            kendaraan_ids = self.search([('no_mesin', '=', no_mesin)])
            if kendaraan_ids:
                result = kendaraan_ids.read(read_list)
            else:
                result = []
            return result, len(kendaraan_ids.exists())
        search_list = []
        kendaraan_ids = self.search(search_list, offset=offset, limit=limit)
        result = []
        for kendaraan in kendaraan_ids:
            sidik_jari = []
            for sj in kendaraan.sidik_jari_ids:
                sidik_jari.append({
                    'id': sj.id,
                    'id_finger': sj.id_finger,
                    'nama': sj.partner_id.name,
                })
            hasil = kendaraan.read(read_list)
            hasil[0]['sidik_jari_ids'] = sidik_jari
            result.append(hasil[0])
        return result, len(kendaraan_ids.exists())

    def api_get_kendaraan_masuk(self, no_mesin=False, offset=0, limit=None):
        read_list = [
            'name',
            'partner_id',
            'no_mesin',
            'sidik_jari_ids',
        ]
        if no_mesin:
            cek_masuk = self.env['bioparks.riwayat'].search([
                ('jenis', '=', 'inout'),
                ('no_mesin', '=', no_mesin),
                ('jam_masuk', '!=', False),
                ('jam_keluar', '=', False)
            ], order="id desc", limit=1)
            if cek_masuk:
                return ([1, 2], 'Anda Belum Keluar!!!')
            kendaraan_ids = self.search([('no_mesin', '=', no_mesin)])
            if kendaraan_ids:
                result = kendaraan_ids.read(read_list)
            else:
                result = []
            return result, len(kendaraan_ids.exists())
        search_list = []
        kendaraan_ids = self.search(search_list, offset=offset, limit=limit)
        result = []
        for kendaraan in kendaraan_ids:
            sidik_jari = []
            for sj in kendaraan.sidik_jari_ids:
                sidik_jari.append({
                    'id': sj.id,
                    'id_finger': sj.id_finger,
                    'nama': sj.partner_id.name,
                })
            hasil = kendaraan.read(read_list)
            hasil[0]['sidik_jari_ids'] = sidik_jari
            result.append(hasil[0])
        return result, len(kendaraan_ids.exists())


class SidikJariAPI(models.Model):
    _inherit = 'bioparks.sidik.jari'

    def api_get_sidik_jari(self, no_mesin=False, sidik_jari_id=False, offset=0, limit=None):
        read_list = [
            'partner_id',
            'kendaraan_id',
        ]
        if no_mesin and sidik_jari_id:
            sidik_jari_ids = self.search(
                [('no_mesin', '=', no_mesin), ('id_finger', '=', sidik_jari_id)])
            result = []
            if sidik_jari_ids:
                result = sidik_jari_ids.read(read_list)
            return result, len(sidik_jari_ids.exists())
        search_list = []
        sidik_jari_ids = self.search(search_list, offset=offset, limit=limit)
        result = sidik_jari_ids.read(read_list)
        return result, len(sidik_jari_ids.exists())


class RiwayatAPI(models.Model):
    _inherit = 'bioparks.riwayat'

    def api_post_riwayat(self, body, **kwargs):
        new_riwayat = body.get('data', False)
        jam_masuk = new_riwayat['jam_masuk']
        jam_masuk_datetime = datetime.strptime(jam_masuk, '%Y-%m-%d %H:%M:%S')
        jam_masuk_id = jam_masuk_datetime - timedelta(hours=7)
        new_riwayat['jam_masuk'] = jam_masuk_id
        uid = kwargs.get('uid', 1)
        if new_riwayat:
            try:
                cek_keluar = self.env['bioparks.riwayat'].sudo().search([
                    ('id', '!=', self.id),
                    ('jenis', '=', 'inout'),
                    ('partner_id', '=', new_riwayat['partner_id']),
                    ('jam_masuk', '!=', False),
                    ('jam_keluar', '=', False)
                ], order="id desc", limit=1)
                riwayat_id = self.with_user(ADMIN_USER)
                if not cek_keluar:
                    riwayat_id = self.with_user(ADMIN_USER).create(new_riwayat)
            except Exception as e:
                se = _serialize_exception(e)
                return 400, {"error": {"code": 400, "message": "Gagal membuat data riwayat.",
                                       "err": se
                                       }}
        return 201, {"riwayat_id": riwayat_id.ids}  # bisa multi

    def api_edit_riwayat(self, no_mesin, body, **kwargs):
        vals = body.get('data', False)
        uid = kwargs.get('uid', 1)
        if vals:
            try:
                riwayat_id = self.with_user(
                    ADMIN_USER).search([('no_mesin', '=', no_mesin), ('jam_keluar', '=', False)], order="id desc", limit=1)
                riwayat_id.with_user(ADMIN_USER).write(vals)
            except Exception as e:
                se = _serialize_exception(e)
                return {"error": {"code": 400, "message": "gagal mengedit data riwayat",
                                  "err": se
                                  }}
        return {"result": "sukses", "riwayat_id": riwayat_id.id}
