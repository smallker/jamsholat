import 'package:flutter/material.dart';
import 'package:flutter_template/model/Config/config_time.dart';
import 'package:flutter_template/model/config/config_auto.dart';
import 'package:flutter_template/services/ble_service.dart';
import 'package:flutter_template/widget/color_material.dart';
import 'package:flutter_template/widget/custom_button.dart';
import 'package:flutter_template/widget/pixel.dart';
import 'package:get/get.dart';
import 'package:google_fonts/google_fonts.dart';
import '../../model/Config/config_wifi.dart';

class MorePage extends StatelessWidget {
  _showDatePicker(BuildContext context) async {
    return await showDatePicker(
      context: context,
      firstDate: DateTime(2015),
      lastDate: DateTime(2100),
      initialDate: DateTime.now(),
    ).then(
      (date) async => date != null
          ? await showTimePicker(
              context: context,
              initialTime: TimeOfDay.fromDateTime(
                DateTime.now(),
              ),
            ).then(
              (time) => BleService.uploadConfig(
                ConfigTime.fromDateTime(
                  date.add(
                    Duration(hours: time.hour, minutes: time.minute),
                  ),
                ).toJson(),
              ),
            )
          : Get.back(),
    );
  }

  _wifiSetting() {
    TextEditingController ssid = TextEditingController();
    TextEditingController pass = TextEditingController();
    return Get.defaultDialog(
      title: 'Pengaturan WiFi',
      content: Container(
        padding: EdgeInsets.all(Pixel.x * 5),
        width: Pixel.x * 80,
        child: Form(
          child: Column(
            children: [
              TextFormField(
                controller: ssid,
                decoration: InputDecoration(
                    hintText: 'SSID WiFi',
                    hoverColor: Colors.white,
                    hintStyle: GoogleFonts.poppins(color: Colors.grey),
                    fillColor: Colors.white,
                    focusColor: Colors.white),
              ),
              TextFormField(
                controller: pass,
                obscureText: true,
                decoration: InputDecoration(
                    hintText: 'Password WiFi',
                    hoverColor: Colors.white,
                    hintStyle: GoogleFonts.poppins(color: Colors.grey),
                    fillColor: Colors.white,
                    focusColor: Colors.white),
              )
            ],
          ),
        ),
      ),
      cancel: FlatButton.icon(
        icon: Icon(Icons.cancel),
        label: Text('Batal'),
        onPressed: () => Get.back(),
      ),
      confirm: FlatButton.icon(
        icon: Icon(Icons.check),
        label: Text('OK'),
        onPressed: () => BleService.uploadConfig(
            ConfigWifi.fill(ssid.text, pass.text).toJson()),
      ),
    );
  }

  _powerSchedule(BuildContext context) async {
    TextEditingController onHour = TextEditingController(text: '3');
    TextEditingController onMinute = TextEditingController(text: '0');
    TextEditingController offHour = TextEditingController(text: '23');
    TextEditingController offMinute = TextEditingController(text: '0');

    validate() {
      return onHour.text.isNullOrBlank ||
              onMinute.text.isNullOrBlank ||
              offHour.text.isNullOrBlank ||
              offMinute.text.isNullOrBlank
          ? Get.snackbar('Tidak boleh kosong', 'Isi semua kolom yang tersedia',
              colorText: Colors.white, backgroundColor: Colors.blue)
          : BleService.uploadConfig(
              ConfigAuto.fromInt(
                int.tryParse(onHour.text),
                int.tryParse(onMinute.text),
                int.tryParse(offHour.text),
                int.tryParse(offMinute.text),
              ).toJson(),
            ).then(
              (value) => Get.back(),
            );
    }

    return Get.defaultDialog(
      title: 'Jadwal nyala & mati',
      confirm: FlatButton.icon(
        icon: Icon(Icons.check_box),
        label: Text('Simpan'),
        onPressed: () => validate(),
      ),
      cancel: FlatButton.icon(
        icon: Icon(Icons.cancel),
        label: Text('Batal'),
        onPressed: () => Get.back(),
      ),
      content: Column(
        children: [
          Text('Set nyala'),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Container(
                width: Pixel.x * 15,
                child: TextField(
                  controller: onHour,
                  keyboardType: TextInputType.number,
                ),
              ),
              Text(':'),
              Container(
                width: Pixel.x * 15,
                child: TextField(
                  controller: onMinute,
                  keyboardType: TextInputType.number,
                ),
              ),
            ],
          ),
          Text('Set mati'),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Container(
                width: Pixel.x * 15,
                child: TextField(
                  controller: offHour,
                  keyboardType: TextInputType.number,
                ),
              ),
              Text(':'),
              Container(
                width: Pixel.x * 15,
                child: TextField(
                  controller: offMinute,
                  keyboardType: TextInputType.number,
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    Widget _title() {
      return Positioned(
        top: Pixel.y * 10,
        left: Pixel.x * 14,
        child: Text(
          'Pengaturan',
          style:
              GoogleFonts.poppins(color: Colors.white, fontSize: Pixel.x * 5),
        ),
      );
    }

    Widget _item({IconData icon, String hint, Function function}) {
      return Expanded(
        flex: 1,
        child: GestureDetector(
          onTap: function,
          child: Container(
            width: Pixel.x * 100,
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.start,
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                Expanded(
                  flex: 1,
                  child: Icon(
                    icon,
                    color: Colors.white,
                  ),
                ),
                Expanded(
                  flex: 6,
                  child: Text(hint,
                      style: GoogleFonts.poppins(
                          color: Colors.white, fontSize: Pixel.x * 5)),
                )
              ],
            ),
          ),
        ),
      );
    }

    Widget _menu() {
      return Positioned(
        top: Pixel.y * 20,
        left: Pixel.x * 10,
        width: Pixel.x * 100,
        height: Pixel.y * 30,
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            _item(
              icon: Icons.access_time,
              hint: 'Atur jam tanggal',
              function: () => _showDatePicker(context),
            ),
            _item(
                icon: Icons.wifi,
                hint: 'Ganti password wifi',
                function: () => _wifiSetting()),
            _item(
                icon: Icons.queue_music,
                hint: 'Set timer murrotal',
                function: null),
            _item(icon: Icons.timer, hint: 'Set timer iqomah', function: null),
            _item(
              icon: Icons.power_settings_new,
              hint: 'Jadwal mati & nyala',
              function: () => _powerSchedule(context),
            )
          ],
        ),
      );
    }

    return Container(
      color: ColorMaterial.green,
      child: Stack(
        children: [_title(), _menu()],
      ),
    );
  }
}
