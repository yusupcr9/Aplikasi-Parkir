from odoo import http
from odoo.http import request, route
from datetime import date, datetime, timedelta
from odoo.addons.rest_api.jwt.login import token_required

ADMIN_USER = 1


class APIKendaraan(http.Controller):

    @route(route=[
        '/api/v1/kendaraan',
        '/api/v1/kendaraan/<string:no_mesin>',
    ],
        methods=['GET'], type='json', auth='public', csrf=False)
    def get_my_kendaraan(self, no_mesin=False, debug=False, **kwargs):
        kendaraan = request.env['bioparks.kendaraan'].with_user(
            ADMIN_USER).api_get_kendaraan(no_mesin=no_mesin)
        result = {}
        result['result'] = kendaraan
        return result

    @route(route=[
        '/api/v1/kendaraan-masuk/<string:no_mesin>',
    ],
        methods=['GET'], type='json', auth='public', csrf=False)
    def get_my_kendaraan_masuk(self, no_mesin=False, debug=False, **kwargs):
        kendaraan = request.env['bioparks.kendaraan'].with_user(
            ADMIN_USER).api_get_kendaraan_masuk(no_mesin=no_mesin)
        result = {}
        result['result'] = kendaraan
        return result

    @route(route=[
        '/api/v1/sidik-jari',
        '/api/v1/sidik-jari/<string:no_mesin>/<int:sidik_jari_id>',
    ],
        methods=['GET'], type='json', auth='public', csrf=False)
    def get_my_sidik_jari(self, no_mesin=False, sidik_jari_id=False, debug=False, **kwargs):
        sidik_jari = request.env['bioparks.sidik.jari'].with_user(
            ADMIN_USER).api_get_sidik_jari(no_mesin=no_mesin, sidik_jari_id=sidik_jari_id)
        result = {}
        result['result'] = sidik_jari
        return result

    @route('/api/v1/riwayat', methods=['POST'], type='json', auth='public', csrf=False)
    def post_riwayat(self, **kwargs):
        body = request.jsonrequest
        code, result = request.env['bioparks.riwayat'].api_post_riwayat(
            body, **kwargs)
        return {'result': result, 'code': code}

    @route('/api/v1/riwayat/<string:no_mesin>', methods=['PUT'], type='json', auth='public', csrf=False)
    def edit_riwayat(self, no_mesin, **kwargs):
        body = request.jsonrequest
        result = request.env['bioparks.riwayat']\
            .api_edit_riwayat(no_mesin, body, **kwargs)
        return {'result': result, 'code': 201}

    @route(route=[
        '/api/v1/data-monitoring',
    ],
        methods=['GET'], type='json', auth='public', csrf=False)
    def get_my_monitoring(self,  debug=False, **kwargs):
        min_time = datetime.min.time()
        max_time = datetime.max.time()
        today = date.today()
        datetime_min = datetime.combine(today, min_time) - timedelta(hours=7)
        datetime_max = datetime.combine(today, max_time) - timedelta(hours=7)
        nilai_masuk = nilai_keluar = nilai_parkir = kuota = 0
        masuk = request.env['bioparks.riwayat'].sudo().search([
            ('jenis', '=', 'inout'),
            ('jam_masuk', '>=', datetime_min),
            ('jam_masuk', '<=', datetime_max)
        ])
        if masuk:
            nilai_masuk = len(masuk)
        keluar = request.env['bioparks.riwayat'].sudo().search([
            ('jenis', '=', 'inout'),
            ('jam_masuk', '>=', datetime_min),
            ('jam_masuk', '<=', datetime_max),
            ('jam_keluar', '!=', False)
        ])
        if keluar:
            nilai_keluar = len(keluar)
        nilai_parkir = nilai_masuk-nilai_keluar
        pengaturan = request.env['bioparks.pengaturan'].sudo().search(
            [('active', '=', True)], limit=1)
        if pengaturan:
            kuota = pengaturan.kuota - nilai_parkir
        hasil = []
        vals = {
            'masuk': nilai_masuk,
            'keluar': nilai_keluar,
            'parkir': nilai_parkir,
            'kuota': kuota,
        }
        hasil.append(vals)
        result = {}
        result['result'] = hasil[0]
        return result
