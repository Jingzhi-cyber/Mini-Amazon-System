# Generated by Django 4.1.5 on 2023-04-26 17:22

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('amazonSite', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='package',
            name='warehouse_id',
            field=models.IntegerField(default=0),
            preserve_default=False,
        ),
    ]