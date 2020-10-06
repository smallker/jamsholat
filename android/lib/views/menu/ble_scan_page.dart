import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:flutter_template/controller/ble_scan_ctl.dart';
import 'package:flutter_template/services/api_repository.dart';
import 'package:flutter_template/services/ble_service.dart';
import 'package:flutter_template/widget/pixel.dart';
import 'package:get/get.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:load/load.dart';

class BleScanPage extends StatefulWidget {
  @override
  _BleScanPageState createState() => _BleScanPageState();
}

class _BleScanPageState extends State<BleScanPage> {
  Widget _listBle(List<BluetoothDevice> ble) {
    return Container(
      width: Pixel.x * 80,
      height: Pixel.y * 20,
      child: ListView.builder(
        itemCount: ble.length,
        itemBuilder: (BuildContext context, int index) {
          return Wrap(
            children: [
              FlatButton.icon(
                icon: Icon(Icons.bluetooth),
                label: Text(
                  ble[index].name.length > 0 ? '${ble[index].name}' : 'no name',
                  style: GoogleFonts.poppins(fontSize: Pixel.x * 5),
                  textAlign: TextAlign.start,
                ),
                onPressed: () =>
                    BleService.connect(ble[index]).whenComplete(() {
                  Get.back();
                  Future.delayed(
                    Duration(seconds: 5),
                    () => BleService.getdata().then(
                      (value) {
                        hideLoadingDialog();
                        ApiRepository.getschedule();
                      },
                    ),
                  );
                }),
              )
            ],
          );
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return GetBuilder<BleScanCtl>(
      init: BleScanCtl(),
      initState: (state) {
        BleService.scan();
      },
      builder: (ble) {
        return ble.isScanning
            ? CircularProgressIndicator()
            : Container(
                child: ble.ble.length == 0 ? Text('kosong') : _listBle(ble.ble),
              );
      },
    );
  }
}
