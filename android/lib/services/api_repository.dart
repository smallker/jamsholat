import 'package:flutter_template/controller/city_list_ctl.dart';
import 'package:flutter_template/controller/pray_ctl.dart';
import 'package:flutter_template/model/city_list.dart';
import 'package:flutter_template/model/pray_schedule.dart';
import 'package:get/get.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:intl/intl.dart';

class ApiRepository {
  static getcity() async {
    final ctl = Get.put(CityListCtl());
    var url = 'https://api.banghasan.com/sholat/format/json/kota';
    var response = await http.get(url);
    var result = CityList.fromJson(json.decode(response.body));
    ctl.updateList(result);
  }

  static getschedule() async {
    final String date = DateFormat('yyyy-MM-dd').format(DateTime.now());
    final city = Get.put(CityListCtl());
    final pray = Get.put(PrayCtl());
    var url =
        'https://api.banghasan.com/sholat/format/json/jadwal/kota/${city.kota.id}/tanggal/$date';
    var response = await http.get(url);
    try {
      var result = PraySchedule.fromJson(json.decode(response.body));
      if (city.kota.id != '0') pray.updateSchedule(result);
    } catch (e) {
      print(e);
    }
  }
}