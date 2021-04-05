from odoo import api, fields, models, _
from datetime import date, datetime, timedelta


class ReportRiwayat(models.TransientModel):
    _name = 'bioparks.riwayat.report'
    _description = 'Wizard Report Riwayat'

    start_date = fields.Date(string='Dari Tanggal', required=True)
    end_date = fields.Date(string='Sampai Tanggal', required=True)

    def get_report(self):
        data = {'date_start': self.start_date,
                'date_stop': self.end_date,
                'model': self._name}
        return self.env.ref('yusup_bioparks.report_riwayat').report_action([], data=data)


class ReportRiwayatAbstrak(models.AbstractModel):
    _name = 'report.yusup_bioparks.report_riwayat_dokumen'

    @api.model
    def _get_report_values(self, docids, data=None):
        company = self.env.company
        date_start = datetime.strptime(data['date_start'], '%Y-%m-%d')
        date_end = datetime.strptime(data['date_stop'], '%Y-%m-%d')

        min_time = datetime.min.time()
        max_time = datetime.max.time()

        datetime_start_min = datetime.combine(date_start, min_time)
        datetime_start_min_id = datetime_start_min - timedelta(hours=7)

        datetime_end_max = datetime.combine(date_end, max_time)
        datetime_end_max_id = datetime_end_max - timedelta(hours=7)

        riwayats = self.env['bioparks.riwayat'].sudo().search([
            ('jam_masuk', '>=', datetime_start_min_id),
            ('jam_masuk', '<=', datetime_end_max_id)
        ], order="jam_masuk desc")

        masuk = len(riwayats)
        keluar = len(riwayats.filtered(lambda m: m.jam_keluar != False))
        parkir = masuk-keluar

        docs = []
        for riwayat in riwayats:
            jam_masuk = riwayat.jam_masuk
            jam_keluar = riwayat.jam_keluar
            if riwayat.jam_masuk:
                jam_masuk = riwayat.jam_masuk + timedelta(hours=7)
            if riwayat.jam_keluar:
                jam_keluar = riwayat.jam_keluar + timedelta(hours=7)
            docs.append({
                'nama': riwayat.partner_id.name,
                'jam_masuk': jam_masuk,
                'jam_keluar': jam_keluar
            })
        value = {
            'doc_ids': docids,
            'doc_model': data['model'],
            'date_start': date_start.strftime('%d %B %Y'),
            'date_end': date_end.strftime('%d %B %Y'),
            'masuk': masuk,
            'keluar': keluar,
            'parkir': parkir,
            'docs': docs,
            'company': company
        }
        return value
