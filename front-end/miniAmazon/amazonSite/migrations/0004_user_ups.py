# Generated by Django 4.0.8 on 2023-04-28 02:26

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('amazonSite', '0003_remove_package_track_num_package_truck_id'),
    ]

    operations = [
        migrations.CreateModel(
            name='user_ups',
            fields=[
                ('user_id', models.IntegerField(primary_key=True, serialize=False)),
                ('ups_id', models.IntegerField(blank=True, null=True)),
            ],
        ),
    ]
