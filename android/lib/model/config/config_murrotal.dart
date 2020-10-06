import 'dart:convert';

class ConfigMurrotal {
  ConfigMurrotal({
    this.config,
    this.timer,
  });

  int config;
  int timer;

  factory ConfigMurrotal.fromJson(String str) =>
      ConfigMurrotal.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigMurrotal.fromMap(Map<String, dynamic> json) => ConfigMurrotal(
        config: json["config"],
        timer: json["timer"],
      );
  factory ConfigMurrotal.fromInt(int timer) =>
      ConfigMurrotal(config: 6, timer: timer);
  Map<String, dynamic> toMap() => {
        "config": config,
        "timer": timer,
      };
}
