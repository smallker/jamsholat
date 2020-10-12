import 'package:geolocator/geolocator.dart';
import 'package:get/get.dart';

class GeolocatorInitService {
  static Future<void> init() async {
    try {
      Position position =
          await getCurrentPosition(desiredAccuracy: LocationAccuracy.high);
      print(position);
    } catch (e) {
      Get.snackbar(
        'Lokasi tidak aktif',
        'BLE perlu akses lokasi',
      );
      print(e);
    }
  }
}
