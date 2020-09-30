import 'package:flutter/material.dart';
import 'package:flutter_template/widget/color_material.dart';
import 'package:flutter_template/widget/pixel.dart';
import 'package:google_fonts/google_fonts.dart';

class MorePage extends StatelessWidget {
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
                    icon: Icons.access_time, hint: 'Atur jam', function: null),
                _item(
                    icon: Icons.wifi,
                    hint: 'Ganti password wifi',
                    function: () => print('p')),
                _item(
                    icon: Icons.queue_music,
                    hint: 'Set timer murrotal',
                    function: null),
                _item(
                    icon: Icons.timer, hint: 'Set timer iqomah', function: null)
              ]));
    }

    return Container(
      color: ColorMaterial.green,
      child: Stack(
        children: [_title(), _menu()],
      ),
    );
  }
}
