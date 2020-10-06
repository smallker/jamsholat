import 'dart:convert';

class ConfigWifi {
  ConfigWifi({
    this.config,
    this.ssid,
    this.password,
  });

  int config;
  String ssid;
  String password;

  factory ConfigWifi.fromJson(String str) =>
      ConfigWifi.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigWifi.fromMap(Map<String, dynamic> json) => ConfigWifi(
        config: json["config"],
        ssid: json["ssid"],
        password: json["password"],
      );

  Map<String, dynamic> toMap() => {
        "config": config,
        "ssid": ssid,
        "password": password,
      };
  factory ConfigWifi.fill(String ssid, String pass) =>
      ConfigWifi(config: 2, ssid: ssid, password: pass);
}
