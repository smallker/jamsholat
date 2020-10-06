import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_template/controller/ble_state_ctl.dart';
import 'package:flutter_template/services/ble_service.dart';
import 'package:flutter_template/widget/color_material.dart';
import 'package:flutter_template/widget/pixel.dart';
import 'package:flutter_template/views/menu/home_page.dart';
import 'package:flutter_template/views/menu/more_page.dart';
import 'package:persistent_bottom_nav_bar/persistent-tab-view.dart';
import 'package:get/get.dart';

import 'menu/ble_scan_page.dart';

class MenuPage extends StatefulWidget {
  @override
  _MenuPageState createState() => _MenuPageState();
}

class _MenuPageState extends State<MenuPage> {
  final ctl = Get.put(BleStateCtl());
  PersistentTabController controller;
  Widget _floatingActionButton() {
    return FloatingActionButton(
      backgroundColor: ColorMaterial.green,
      child: Icon(Icons.bluetooth_searching),
      onPressed: () => Get.defaultDialog(
        title: 'JWS Tersedia',
        content: GetBuilder<BleStateCtl>(
            init: BleStateCtl(),
            builder: (ble) {
              return ble.connected
                  ? ListTile(
                      title: Text(ble.device.name),
                      leading: Icon(Icons.bluetooth),
                      subtitle: Text('Terhubung'),
                      onTap: () => Get.defaultDialog(
                          title: 'Putuskan sambungan?',
                          content:
                              Text('Sambungan ke perangkat ini akan terputus'),
                          confirm: FlatButton(
                            child: Text('Putus'),
                            onPressed: () =>
                                BleService.disconnect(ble.device).then(
                              (_) => Get.back(),
                            ),
                          ),
                          cancel: FlatButton(
                            child: Text('Batal'),
                            onPressed: () => Get.back(),
                          )),
                    )
                  : BleScanPage();
            }),
      ),
      // onPressed: () => BleService.disconnect(BleStateCtl().device),
    );
  }

  List<PersistentBottomNavBarItem> navBarItem = [
    PersistentBottomNavBarItem(
        icon: Icon(Icons.home),
        title: 'Beranda',
        activeColor: ColorMaterial.green),
    PersistentBottomNavBarItem(
        icon: Icon(Icons.settings),
        title: 'Pengaturan',
        activeColor: ColorMaterial.green),
  ];
  @override
  Widget build(BuildContext context) {
    Pixel().init(context);
    return WillPopScope(
      onWillPop: () async => true,
      child: Container(
        color: ColorMaterial.green,
        child: SafeArea(
          child: PersistentTabView(
            controller: controller,
            screens: [HomePage(), MorePage()],
            items: navBarItem,
            navBarHeight: Pixel.y * 10,
            confineInSafeArea: true,
            backgroundColor: Colors.white,
            handleAndroidBackButtonPress: true,
            resizeToAvoidBottomInset: true,
            stateManagement: true,
            hideNavigationBarWhenKeyboardShows: true,
            decoration: NavBarDecoration(
                // borderRadius: BorderRadius.circular(10.0),
                colorBehindNavBar: Colors.white,
                boxShadow: [
                  BoxShadow(
                      color: Colors.white.withOpacity(0.3),
                      blurRadius: 3,
                      spreadRadius: 3)
                ]),
            popAllScreensOnTapOfSelectedTab: true,
            itemAnimationProperties: ItemAnimationProperties(
              duration: Duration(milliseconds: 200),
              curve: Curves.ease,
            ),
            screenTransitionAnimation: ScreenTransitionAnimation(
              animateTabTransition: true,
              curve: Curves.ease,
              duration: Duration(milliseconds: 200),
            ),
            navBarStyle: NavBarStyle.style1,
            floatingActionButton: _floatingActionButton(),
          ),
        ),
      ),
    );
  }
}
