import 'dart:convert';

class ConfigIqomah {
  ConfigIqomah({
    this.config,
    this.timer,
  });

  int config;
  int timer;

  factory ConfigIqomah.fromJson(String str) =>
      ConfigIqomah.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigIqomah.fromMap(Map<String, dynamic> json) => ConfigIqomah(
        config: json["config"],
        timer: json["timer"],
      );
  factory ConfigIqomah.fromInt(int timer) =>
      ConfigIqomah(config: 5, timer: timer);
  Map<String, dynamic> toMap() => {
        "config": config,
        "timer": timer,
      };
}
