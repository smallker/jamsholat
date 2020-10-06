import 'package:flutter/material.dart';
import 'package:flutter_template/controller/city_list_ctl.dart';
import 'package:flutter_template/controller/pray_ctl.dart';
import 'package:flutter_template/model/pray_schedule.dart';
import 'package:flutter_template/services/api_repository.dart';
import 'package:flutter_template/widget/color_material.dart';
import 'package:flutter_template/widget/pixel.dart';
import 'package:get/get.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:intl/intl.dart';
import 'package:load/load.dart';

class HomePage extends StatefulWidget {
  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _city = Get.put(CityListCtl());
  final _pray = Get.put(PrayCtl());

  void initState() {
    super.initState();
    ApiRepository.getcity();
  }

  @override
  Widget build(BuildContext context) {
    Widget _listview() {
      return GetBuilder(
        builder: (_) {
          return Container(
            width: Pixel.x * 80,
            height: Pixel.y * 50,
            child: ListView.builder(
              itemCount: _city.list.kota.length,
              itemBuilder: (BuildContext context, int index) {
                return FlatButton(
                  onPressed: () {
                    showLoadingDialog();
                    _city.updateKota(_city.list.kota[index]);
                    ApiRepository.getschedule();
                    Get.back();
                    print(prayScheduleToJson(_pray.schedule));
                  },
                  child: Text(_city.list.kota[index].nama),
                );
              },
            ),
          );
        },
      );
    }

    Widget _top() {
      return Expanded(
        flex: 2,
        child: Center(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              GetBuilder<CityListCtl>(builder: (_) {
                return Text(
                  _city.kota.nama,
                  style: GoogleFonts.poppins(
                      color: Colors.white, fontSize: Pixel.x * 7),
                );
              }),
              GestureDetector(
                onTap: () => Get.defaultDialog(
                  title: 'Pilih kota',
                  content: _listview(),
                ),
                child: Text(
                  'Ganti kota',
                  style: GoogleFonts.poppins(
                      color: Colors.white, fontSize: Pixel.x * 5),
                ),
              ),
            ],
          ),
        ),
      );
    }

    Widget _date(String date) {
      return Expanded(
        flex: 1,
        child: Center(
          child: Text(
            date,
            style: GoogleFonts.poppins(fontSize: Pixel.x * 5),
          ),
        ),
      );
    }

    Widget _schedule({String name, String time, Function function}) {
      return Expanded(
        flex: 1,
        child: Container(
          width: Pixel.x * 70,
          child: Row(
            children: [
              Expanded(
                flex: 4,
                child: Text(name),
              ),
              Expanded(
                flex: 2,
                child: Text(
                  time,
                  textAlign: TextAlign.end,
                ),
              ),
              Expanded(
                flex: 1,
                child: GestureDetector(
                  onTap: function,
                  child: Icon(
                    Icons.edit,
                    size: Pixel.x * 4,
                  ),
                ),
              )
            ],
          ),
        ),
      );
    }

    Widget _bottom() {
      return Expanded(
        flex: 3,
        child: Container(
          padding: EdgeInsets.all(Pixel.x * 10),
          width: Pixel.x * 100,
          decoration: BoxDecoration(
              color: Colors.white,
              boxShadow: [
                BoxShadow(
                    color: Colors.white.withOpacity(0.3),
                    blurRadius: 3,
                    spreadRadius: 3)
              ],
              borderRadius: BorderRadius.only(
                  topLeft: Radius.circular(Pixel.x * 5),
                  topRight: Radius.circular(Pixel.x * 5))),
          child: GetBuilder<PrayCtl>(builder: (_) {
            if (_pray.schedule.isNull)
              return Center(
                child: Text('Kosong'),
              );
            else {
              var tanggal = _pray.schedule.query.tanggal;
              var jadwal = _pray.schedule.jadwal.data;
              return Column(
                children: [
                  _date(
                      DateFormat('EEE, dd MMM yyy').format(tanggal.toLocal())),
                  _schedule(name: 'Dzuhur', time: jadwal.dzuhur),
                  _schedule(name: 'Ashar', time: jadwal.ashar),
                  _schedule(name: 'Maghrib', time: jadwal.maghrib),
                  _schedule(name: 'Isya', time: jadwal.isya),
                  _schedule(name: 'Imsak', time: jadwal.imsak),
                  _schedule(name: 'Subuh', time: jadwal.subuh)
                ],
              );
            }
          }),
        ),
      );
    }

    return Container(
      color: ColorMaterial.green,
      child: Column(
        children: [
          _top(),
          _bottom(),
        ],
      ),
    );
  }
}
