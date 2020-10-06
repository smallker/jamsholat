import 'dart:convert';

class ConfigAdzan {
  ConfigAdzan({
    this.config,
    this.timer,
  });

  int config;
  int timer;

  factory ConfigAdzan.fromJson(String str) =>
      ConfigAdzan.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigAdzan.fromMap(Map<String, dynamic> json) => ConfigAdzan(
        config: json["config"],
        timer: json["timer"],
      );
  factory ConfigAdzan.fromInt(int timer) =>
      ConfigAdzan(config: 4, timer: timer);
  Map<String, dynamic> toMap() => {
        "config": config,
        "timer": timer,
      };
}
