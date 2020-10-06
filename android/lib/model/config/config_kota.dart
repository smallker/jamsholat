// To parse this JSON data, do
//
//     final configKota = configKotaFromMap(jsonString);

import 'dart:convert';

class ConfigKota {
  ConfigKota({
    this.config,
    this.kota,
  });

  int config;
  int kota;

  factory ConfigKota.fromJson(String str) =>
      ConfigKota.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigKota.fromMap(Map<String, dynamic> json) => ConfigKota(
        config: json["config"],
        kota: json["kota"],
      );
  factory ConfigKota.fromId(int id) => ConfigKota(config: 7, kota: id);
  Map<String, dynamic> toMap() => {
        "config": config,
        "kota": kota,
      };
}
