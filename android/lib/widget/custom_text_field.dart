import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:form_field_validator/form_field_validator.dart';
import 'package:google_fonts/google_fonts.dart';

class CustomTextField extends StatefulWidget {
  final TextFieldValidator validator;
  final TextEditingController controller;
  final double width;
  final bool secure;
  final String hint;
  final IconData icon;
  const CustomTextField(
      {Key key,
      this.validator,
      this.controller,
      this.secure,
      this.hint,
      this.width,
      this.icon})
      : super(key: key);
  @override
  _CustomTextFieldState createState() => _CustomTextFieldState();
}

class _CustomTextFieldState extends State<CustomTextField> {
  @override
  Widget build(BuildContext context) {
    return Container(
      width: widget.width,
      child: TextFormField(
          validator: widget.validator,
          controller: widget.controller,
          obscureText: widget.secure,
          decoration: InputDecoration(
              icon: Icon(widget.icon),
              hintText: widget.hint,
              hoverColor: Colors.white,
              hintStyle: GoogleFonts.poppins(color: Colors.grey),
              fillColor: Colors.white,
              alignLabelWithHint: true,
              focusColor: Colors.white)),
    );
  }
}
