// To parse this JSON data, do
//
//     final configAuto = configAutoFromMap(jsonString);

import 'dart:convert';

class ConfigAuto {
  ConfigAuto({
    this.config,
    this.offHour,
    this.offMinute,
    this.onHour,
    this.onMinute,
  });

  int config;
  int offHour;
  int offMinute;
  int onHour;
  int onMinute;

  factory ConfigAuto.fromJson(String str) =>
      ConfigAuto.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigAuto.fromMap(Map<String, dynamic> json) => ConfigAuto(
        config: json["config"],
        offHour: json["off_hour"],
        offMinute: json["off_minute"],
        onHour: json["on_hour"],
        onMinute: json["on_minute"],
      );
  factory ConfigAuto.fromInt(
          int onHour, int onMinute, int offHour, int offMinute) =>
      ConfigAuto(
          config: 3,
          onHour: onHour,
          onMinute: onMinute,
          offHour: offHour,
          offMinute: offMinute);
  Map<String, dynamic> toMap() => {
        "config": config,
        "off_hour": offHour,
        "off_minute": offMinute,
        "on_hour": onHour,
        "on_minute": onMinute,
      };
}
