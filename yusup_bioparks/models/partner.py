from odoo import api, fields, models


class InheritPartner(models.Model):
    _inherit = 'res.partner'

    kendaraan_id = fields.Many2one(
        comodel_name='bioparks.kendaraan', string='Kendaraan')
