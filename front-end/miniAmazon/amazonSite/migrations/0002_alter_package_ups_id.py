# Generated by Django 4.0.8 on 2023-04-28 01:20

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('amazonSite', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='package',
            name='ups_id',
            field=models.IntegerField(blank=True, null=True),
        ),
    ]