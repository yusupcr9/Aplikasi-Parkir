# -*- coding: utf-8 -*-
import json

from datetime import datetime, timedelta, date

from odoo import fields, http, _
from odoo.http import content_disposition, request


class RiwayatBioparks(http.Controller):

    @http.route([
        '''/bioparks/unsika/'''
    ], type='http', methods=['GET', 'POST'], auth="public", website=True, sitemap=False)
    def bioparks_unsika(self, **post):
        current_date = fields.Date.today()
        min_time = datetime.min.time()
        max_time = datetime.max.time()
        datetime_min = datetime.combine(current_date, min_time)
        datetime_max = datetime.combine(current_date, max_time)
        riwayats = request.env['bioparks.riwayat'].sudo().search([
            ('jam_keluar', '>=', datetime_min),
            ('jam_keluar', '<=', datetime_max)
        ])
        values = {
            'riwayats': riwayats,
            'current_date': current_date
        }
        return request.render("yusup_bioparks.bioparks_riwayat", values)

    @http.route('/bioparks/data-riwayat', auth='public', type='http',  methods=['GET'], website=True)
    def data_json_riwayat(self, **kw):
        # incidents = request.env['jidoka.incident'].sudo().search(
        #     [('state', 'in', ('open', 'process'))])
        # flds = ['name', 'create_uid', 'site_id']
        now = datetime.now()
        now_min = now.strftime('%Y-%m-%d 00:00:00')
        now_max = now.strftime('%Y-%m-%d 23:59:59')
        query = """select
                        p.name,
                        r.jam_keluar
                    from bioparks_riwayat r
                    join res_partner p on r.partner_id = p.id
                    where r.jam_keluar between '%s' and '%s' and r.jenis='out'
                    """
        request.env.cr.execute(query % (now_min, now_max))
        query_result = request.env.cr.dictfetchall()
        for qr in query_result:
            jam_keluar_id = qr['jam_keluar'] + timedelta(hours=7)
            qr['jam_keluar'] = jam_keluar_id.strftime(
                '%Y-%m-%d %H:%M:%S')
        return json.dumps({'riwayats': query_result})

    @http.route([
        '''/tim-bioparks/unsika/'''
    ], type='http', methods=['GET', 'POST'], auth="public", website=True, sitemap=False)
    def bioparks_unsika_in_out(self, **post):
        current_date = fields.Date.today()
        min_time = datetime.min.time()
        max_time = datetime.max.time()
        datetime_min = datetime.combine(current_date, min_time)
        datetime_max = datetime.combine(current_date, max_time)
        riwayats = request.env['bioparks.riwayat'].sudo().search([
            ('jam_masuk', '>=', datetime_min),
            ('jam_masuk', '<=', datetime_max)
        ])
        values = {
            'riwayats': riwayats,
            'current_date': current_date
        }
        return request.render("yusup_bioparks.bioparks_riwayat_inout", values)

    @http.route('/bioparks/data-riwayat-in-out', auth='public', type='http',  methods=['GET'], website=True)
    def data_json_riwayat_in_out(self, **kw):
        # incidents = request.env['jidoka.incident'].sudo().search(
        #     [('state', 'in', ('open', 'process'))])
        # flds = ['name', 'create_uid', 'site_id']
        min_time = datetime.min.time()
        max_time = datetime.max.time()
        today = date.today()
        datetime_min = datetime.combine(today, min_time) - timedelta(hours=7)
        datetime_max = datetime.combine(today, max_time) - timedelta(hours=7)
        query = """select
                        p.name,
                        r.jam_masuk,
                        r.jam_keluar
                    from bioparks_riwayat r
                    join res_partner p on r.partner_id = p.id
                    where r.jam_masuk between '%s' and '%s' and r.jenis='inout'
                    order by r.jam_masuk desc
                    """
        request.env.cr.execute(query % (datetime_min, datetime_max))
        query_result = request.env.cr.dictfetchall()
        for qr in query_result:
            jam_keluar_id = ''
            if qr['jam_keluar']:
                jam_keluar_id = qr['jam_keluar'] + timedelta(hours=7)
                qr['jam_keluar'] = jam_keluar_id.strftime('%Y-%m-%d %H:%M:%S')
            jam_masuk_id = qr['jam_masuk'] + timedelta(hours=7)
            qr['jam_masuk'] = jam_masuk_id.strftime('%Y-%m-%d %H:%M:%S')
        return json.dumps({'riwayats': query_result})

    @http.route('/bioparks/data-monitoring', auth='public', type='http',  methods=['GET'], website=True)
    def data_json_monitoring(self, **kw):
        # incidents = request.env['jidoka.incident'].sudo().search(
        #     [('state', 'in', ('open', 'process'))])
        # flds = ['name', 'create_uid', 'site_id']
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
        result = []
        vals = {
            'masuk': nilai_masuk,
            'keluar': nilai_keluar,
            'parkir': nilai_parkir,
            'kuota': kuota,
        }
        result.append(vals)
        return json.dumps({'monitor': result})
