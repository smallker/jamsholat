import 'package:flutter/material.dart';
import 'package:flutter_template/services/geolocator_init_service.dart';
import 'package:flutter_template/views/menu_page.dart';
import 'package:get/get.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:load/load.dart';

void main() {
  runApp(LoadingProvider(child: MainApp()));
}

class MainApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    GeolocatorInitService.init();
    return GetMaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'JWS Controller',
      theme: ThemeData(
          visualDensity: VisualDensity.adaptivePlatformDensity,
          textTheme: GoogleFonts.poppinsTextTheme()),
      home: MenuPage(),
    );
  }
}
