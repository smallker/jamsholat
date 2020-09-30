import 'package:flutter_blue/flutter_blue.dart';
import 'package:get/get.dart';

class BleScanCtl extends GetxController {
  List<BluetoothDevice> ble = [];
  bool isScanning = false;

  void updateDevice(List<BluetoothDevice> _ble) {
    ble = _ble;
    reload();
  }

  void updateState(bool _isScanning) {
    isScanning = _isScanning;
    reload();
  }

  void reload() {
    update();
  }
}
