import 'package:flutter_blue/flutter_blue.dart';
import 'package:get/get.dart';

class BleStateCtl extends GetxController {
  BluetoothDevice device;
  bool connected = false;
  updateDevice(BluetoothDevice _device) {
    device = _device;
    reload();
  }

  updateState(bool state) {
    connected = state;
    reload();
  }

  reload() {
    update();
  }
}
