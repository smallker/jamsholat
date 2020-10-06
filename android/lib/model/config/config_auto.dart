// To parse this JSON data, do
//
//     final configAuto = configAutoFromMap(jsonString);

import 'dart:convert';

class ConfigAuto {
  ConfigAuto({
    this.config,
    this.scheduleOff,
    this.scheduleOn,
  });

  int config;
  List<int> scheduleOff;
  List<int> scheduleOn;

  factory ConfigAuto.fromJson(String str) =>
      ConfigAuto.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigAuto.fromMap(Map<String, dynamic> json) => ConfigAuto(
        config: json["config"],
        scheduleOff: List<int>.from(json["schedule_off"].map((x) => x)),
        scheduleOn: List<int>.from(json["schedule_on"].map((x) => x)),
      );
  factory ConfigAuto.fromInt(
          int hourOn, int minuteOn, int hourOff, int minuteOff) =>
      ConfigAuto(
          config: 3,
          scheduleOn: [hourOn, minuteOn],
          scheduleOff: [hourOff, minuteOff]);
  Map<String, dynamic> toMap() => {
        "config": config,
        "schedule_off": List<dynamic>.from(scheduleOff.map((x) => x)),
        "schedule_on": List<dynamic>.from(scheduleOn.map((x) => x)),
      };
}
