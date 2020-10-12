import 'dart:convert';

class PraySchedule {
  PraySchedule({
    this.config,
    this.status,
    this.query,
    this.jadwal,
  });

  int config;
  String status;
  Query query;
  Jadwal jadwal;

  factory PraySchedule.fromJson(String str) =>
      PraySchedule.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory PraySchedule.fromMap(Map<String, dynamic> json) => PraySchedule(
        config: 7,
        status: json["status"],
        query: Query.fromMap(json["query"]),
        jadwal: Jadwal.fromMap(json["jadwal"]),
      );

  Map<String, dynamic> toMap() => {
        "config": 7,
        "status": status,
        "query": query.toMap(),
        "jadwal": jadwal.toMap(),
      };
}

class Jadwal {
  Jadwal({
    this.data,
  });

  Data data;

  factory Jadwal.fromJson(String str) => Jadwal.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory Jadwal.fromMap(Map<String, dynamic> json) => Jadwal(
        data: Data.fromMap(json["data"]),
      );

  Map<String, dynamic> toMap() => {
        "data": data.toMap(),
      };
}

class Data {
  Data({
    this.ashar,
    this.dzuhur,
    this.imsak,
    this.isya,
    this.maghrib,
    this.subuh,
  });

  String ashar;
  String dzuhur;
  String imsak;
  String isya;
  String maghrib;
  String subuh;

  factory Data.fromJson(String str) => Data.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory Data.fromMap(Map<String, dynamic> json) => Data(
        ashar: json["ashar"],
        dzuhur: json["dzuhur"],
        imsak: json["imsak"],
        isya: json["isya"],
        maghrib: json["maghrib"],
        subuh: json["subuh"],
      );

  Map<String, dynamic> toMap() => {
        "ashar": ashar,
        "dzuhur": dzuhur,
        "imsak": imsak,
        "isya": isya,
        "maghrib": maghrib,
        "subuh": subuh,
      };
}

class Query {
  Query({
    this.kota,
    this.tanggal,
  });

  String kota;
  DateTime tanggal;

  factory Query.fromJson(String str) => Query.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory Query.fromMap(Map<String, dynamic> json) => Query(
        kota: json["kota"],
        tanggal: DateTime.parse(json["tanggal"]),
      );

  Map<String, dynamic> toMap() => {
        "kota": kota,
        "tanggal":
            "${tanggal.year.toString().padLeft(4, '0')}-${tanggal.month.toString().padLeft(2, '0')}-${tanggal.day.toString().padLeft(2, '0')}",
      };
}
