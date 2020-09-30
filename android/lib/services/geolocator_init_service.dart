import 'package:geolocator/geolocator.dart';

class GeolocatorInitService {
  static Future<void> init() async {
    // final BleConfigureController _ctl = Get.put(BleConfigureController());
    Position position =
        await getCurrentPosition(desiredAccuracy: LocationAccuracy.high);
    // _ctl.updateLocation(position);
    print(position);
  }
}
