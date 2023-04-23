from django.db import models
from django.contrib.auth.models import User

class Warehouse(models.Model):
    warehouse_id = models.AutoField(primary_key=True, default=0)
    address_x = models.IntegerField(null=True)
    address_y = models.IntegerField(null=True)

    def __str__(self) -> str:
        return "({warehouse_id = self.warehouse_id}, {address_x = self.address_x}, {address_y = self.address_y})".format(self = self)

class Product(models.Model):
    product_id = models.AutoField(primary_key=True, default=0)
    name = models.CharField(max_length=200, null=True)
    description = models.CharField(max_length=1000, null=True)


class Order(models.Model):
    order_num = models.IntegerField()
    product = models.ForeignKey("Product",null=True, on_delete=models.SET_NULL)
    user = models.ForeignKey(User, null=True, on_delete=models.SET_NULL)
    quantity = models.IntegerField()
    order_status = models.CharField(max_length=100)
    package_id = models.IntegerField(null=True, blank=True)
    create_time = models.TimeField(null=True, blank=True)

    class Meta:
        constraints = [
            models.UniqueConstraint(fields=['order_num', 'product'], name='order_product_pk'),
        ]


class Package(models.Model):
    package_id = models.AutoField(primary_key=True, default=0)
    owner = models.ForeignKey(User, null=True, on_delete=models.SET_NULL)
    dest_x = models.IntegerField()
    dest_y = models.IntegerField()
    pack_time = models.TimeField(null=True, blank=True)
    ups_id = models.IntegerField()
    track_num = models.IntegerField()

class Inventory(models.Model):
    warehouse = models.ForeignKey("Warehouse", on_delete=models.CASCADE)
    product = models.ForeignKey("Product", on_delete=models.CASCADE)
    quantity = models.PositiveIntegerField()

    class Meta:
        constraints = [
            models.UniqueConstraint(fields=['warehouse', 'product'], name='warehouse_product_pk'),
        ]