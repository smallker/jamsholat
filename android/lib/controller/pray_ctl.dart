import 'package:flutter_template/model/pray_schedule.dart';
import 'package:get/get.dart';
import 'package:load/load.dart';

class PrayCtl extends GetxController {
  PraySchedule schedule;
  void updateSchedule(PraySchedule _schedule) {
    schedule = _schedule;
    reload();
  }

  reload() {
    hideLoadingDialog();
    update();
  }
}
