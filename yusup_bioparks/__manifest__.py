# -*- coding: utf-8 -*-
{
    'name': "BIOPARKS",

    'summary': """
        Aplikasi BIOPARKS""",

    'description': """
        Aplikasi BIOPARKS
    """,

    'author': "Yusup Firmansyah",
    'website': "http://www.yourcompany.com",

    # Categories can be used to filter modules in modules listing
    # Check https://github.com/odoo/odoo/blob/13.0/odoo/addons/base/data/ir_module_category_data.xml
    # for the full list
    'category': 'Uncategorized',
    'version': '0.1',

    # any module necessary for this one to work correctly
    'depends': ['base', 'website', 'portal'],

    # always loaded
    'data': [
        'security/ir.model.access.csv',
        'security/security.xml',
        'data/ir_sequence_data.xml',
        'reports/external_layout.xml',
        'reports/report.xml',
        'views/kendaraan.xml',
        'views/partner.xml',
        'views/sidik_jari.xml',
        'views/riwayat.xml',
        'views/pengaturan.xml',
        'views/partner.xml',
        'views/templates.xml',
        'views/kartu_template.xml',
        'wizard/wiz_report.xml',
    ],
    # only loaded in demonstration mode
    'demo': [
        'demo/demo.xml',
    ],
    'application': True
}
