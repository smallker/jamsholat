import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:flutter_template/controller/ble_scan_ctl.dart';
import 'package:flutter_template/controller/ble_state_ctl.dart';
import 'package:flutter_template/widget/color_material.dart';
import 'package:get/get.dart';
import 'package:load/load.dart';

class BleService {
  static FlutterBlue flutterBlue = FlutterBlue.instance;
  static Guid characteristicUuid = Guid('beb5483e-36e1-4688-b7f5-ea07361b26a8');
  static Guid servicesUuid = Guid('4fafc201-1fb5-459e-8fcc-c5c9c331914b');
  static Future<void> scan() async {
    final BleScanCtl ctl = Get.put(BleScanCtl());
    flutterBlue.isOn.then((value) {
      if (value) {
        ctl.updateState(true);
        flutterBlue.stopScan();
        flutterBlue.startScan(timeout: Duration(seconds: 5)).whenComplete(() {
          ctl.updateState(false);
          hideLoadingDialog();
        });
        flutterBlue.scanResults.listen((results) {
          List<BluetoothDevice> device = [];
          for (ScanResult r in results) {
            print(r);
            device.add(r.device);
          }
          ctl.updateDevice(device);
        });
      } else
        Get.snackbar('Peringatan', 'Bluetooth mati',
            colorText: ColorMaterial.green, backgroundColor: Colors.white);
    });
  }

  static Future<void> connect(BluetoothDevice device) async {
    final ctl = Get.put(BleStateCtl());
    await device.connect();
    await device.requestMtu(512);
    ctl.updateDevice(device);
    ctl.updateState(true);
  }

  static Future<void> getdata(BluetoothDevice device) async {
    await device.discoverServices().then((services) {
      services.forEach((element) {
        element.characteristics.forEach((element) async {
          if (element.uuid == characteristicUuid) {
            var data = await element.read();
            print(convertToString(data));
          }
        });
      });
    });
  }

  static Future<void> disconnect(BluetoothDevice device) async {
    final ctl = Get.put(BleStateCtl());
    await device.disconnect();
    ctl.updateState(false);
  }

  static List<int> convertToAscii(String data) {
    List<int> list = [];
    for (int i = 0; i < data.length; i++) {
      list.add(data.codeUnitAt(i));
    }
    return list;
  }

  static String convertToString(List<int> data) {
    return String.fromCharCodes(data);
  }

  static Future<void> uploadConfig(String config) async {
    print(config);
    final BleStateCtl _ctl = Get.put(BleStateCtl());
    try {
      List<BluetoothService> services = await _ctl.device.discoverServices();
      services.forEach((service) async {
        if (service.uuid == servicesUuid) {
          service.characteristics.forEach((chara) {
            if (chara.uuid == characteristicUuid) {
              chara.write(convertToAscii(config));
            }
          });
        }
      });
    } catch (e) {
      Get.snackbar('Terputus', 'Hubungkan dengan kontroller terlebih dahulu',
          backgroundColor: Colors.white, colorText: Colors.blue);
    }
  }
}
