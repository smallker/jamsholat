import 'package:flutter_template/model/city_list.dart';
import 'package:get/get.dart';
import 'package:load/load.dart';

class CityListCtl extends GetxController {
  CityList list;
  Kota kota = Kota(id: '0', nama: 'Terputus');
  void updateList(CityList _list) {
    list = _list;
    reload();
  }

  void updateKota(Kota _kota) {
    kota = _kota;
    reload();
  }

  reload() {
    hideLoadingDialog();
    update();
  }
}
