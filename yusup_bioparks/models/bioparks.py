from odoo import api, fields, models, _
from odoo.exceptions import UserError, ValidationError


class DataKendaraan(models.Model):
    _name = 'bioparks.kendaraan'
    _description = 'Data Kendaraan'

    name = fields.Char(string='Nomor Polisi', required=True)
    pemilik_id = fields.Many2one(
        comodel_name='bioparks.pemilik', string='Nama Pemilik')
    partner_id = fields.Many2one(
        comodel_name='res.partner', string='Nama Pemilik', required=True)

    alamat = fields.Text(string='Alamat')
    merk = fields.Char(string='Merek')
    jenis = fields.Selection(string='Jenis', selection=[
                             ('motor', 'Motor'), ('mobil', 'Mobil'), ], required=True)
    tahun = fields.Char(string='Tahun')
    no_mesin = fields.Char(string='Nomor Mesin', required=True)
    foto = fields.Binary(string='Foto')
    sidik_jari_ids = fields.One2many(
        comodel_name='bioparks.sidik.jari', inverse_name='kendaraan_id', string='Data Sidik Jari')

    @api.constrains('partner_id')
    def check_partner_id(self):
        for i in self:
            if i.partner_id:
                partner_awal = self.env['res.partner'].search(
                    [('id', '!=', i.partner_id.id), ('kendaraan_id', '=', i.id)])
                if partner_awal:
                    partner_awal.kendaraan_id = None
                i.partner_id.kendaraan_id = i.id

    _sql_constraints = [
        ('no_mesin_uniq', 'unique(no_mesin)', "Nomor Mesin Sudah Ada !"),
    ]


class DataPemilik(models.Model):
    _name = 'bioparks.pemilik'
    _description = 'Data Pemilik'

    name = fields.Char(string='Nama', required=True)
    kendaraan_id = fields.Many2one(
        comodel_name='bioparks.kendaraan', string='Kendaraan')
    alamat = fields.Text(string='Alamat')


class DataSidikJari(models.Model):
    _name = 'bioparks.sidik.jari'
    _description = 'Data Sidik Jari'

    name = fields.Char(string='Nama', default='New')
    id_finger = fields.Integer(string='ID Fingerprint', required=True)
    partner_id = fields.Many2one(comodel_name='res.partner', string='Nama')
    pemilik_id = fields.Many2one(
        comodel_name='bioparks.pemilik', string='Pemilik')
    kendaraan_id = fields.Many2one(
        comodel_name='bioparks.kendaraan', string='Kendaraan')
    no_mesin = fields.Char(
        string='No Mesin', related="kendaraan_id.no_mesin", store=True)

    @api.onchange('partner_id')
    def _onchange_partner_id(self):
        for record in self:
            if record.partner_id:
                record.name = '{} ({})'.format(
                    record.partner_id.name, record.id_finger)


class History(models.Model):
    _name = 'bioparks.riwayat'
    _description = 'Riwayat BIOPARKS'

    name = fields.Char(string='Nomor', default=lambda self: _('New'))
    partner_id = fields.Many2one(
        comodel_name='res.partner', string='Nama Pemilik')
    pemilik_id = fields.Many2one(
        comodel_name='bioparks.pemilik', string='Nama')
    jam_masuk = fields.Datetime(
        string='Waktu Masuk', default=fields.Datetime.now())
    jam_keluar = fields.Datetime(
        string='Waktu Keluar', )
    jenis = fields.Selection(string='Jenis Data', selection=[
                             ('in', 'Masuk'), ('out', 'Keluar'), ('inout', 'Keluar-Masuk'), ])
    no_mesin = fields.Char(
        string='No Mesin', related="partner_id.kendaraan_id.no_mesin", store=True)

    @api.model
    def create(self, vals):
        if vals.get('name', _('New')) == _('New'):
            vals['name'] = self.env['ir.sequence'].next_by_code(
                'bioparks.riwayat') or _('New')
        result = super(History, self).create(vals)
        return result

    @api.constrains('jam_masuk', 'jam_keluar')
    def _constrains_laporan(self):
        for record in self:
            cek_keluar = self.env['bioparks.riwayat'].search([
                ('id', '!=', record.id),
                ('jenis', '=', 'inout'),
                ('partner_id', '=', record.partner_id.id),
                ('jam_masuk', '!=', False),
                ('jam_keluar', '=', False)
            ], order="id desc", limit=1)
            if record.jam_masuk and cek_keluar and not record.jam_keluar:
                jam = cek_keluar.jam_masuk
                raise ValidationError(
                    _("Anda Belum Keluar Sejak {}".format(jam.strftime('%d %B %Y %H:%M:%S'))))

    @api.onchange('jam_keluar')
    def _onchange_jam_keluar(self):
        for i in self:
            if i.jam_masuk and i.jam_keluar and i.jam_masuk > i.jam_keluar:
                i.jam_masuk = i.jam_keluar

    @api.onchange('jam_masuk')
    def _onchange_jam_masuk(self):
        for i in self:
            if i.jam_keluar and i.jam_masuk and i.jam_masuk > i.jam_keluar:
                i.jam_keluar = i.jam_masuk


class BioparksPengaturan(models.Model):
    _name = 'bioparks.pengaturan'
    _description = 'Pengaturan Bioparks'

    name = fields.Char(string='Name', compute="_compute_name")
    kuota = fields.Integer(string='Kapasitas Maximum Parkiran', required=True)
    jarak = fields.Integer(string='Jarak Maksimal Ultrasonik', required=True)
    active = fields.Boolean(string='Active', default=True)

    @api.constrains('active')
    def _constrains_active(self):
        for i in self:
            cek = self.env['bioparks.pengaturan'].search(
                [('id', '!=', i.id), ('active', '=', True)])
            if i.active and cek:
                cek.active = False

    def _compute_name(self):
        nama = 'Pengaturan ({})'.format(self.id)
        self.name = nama
