import 'dart:convert';

CityList cityListFromJson(String str) => CityList.fromJson(json.decode(str));

String cityListToJson(CityList data) => json.encode(data.toJson());

class CityList {
  CityList({
    this.status,
    this.query,
    this.kota,
  });

  String status;
  Query query;
  List<Kota> kota;

  factory CityList.fromJson(Map<String, dynamic> json) => CityList(
        status: json["status"],
        query: Query.fromJson(json["query"]),
        kota: List<Kota>.from(json["kota"].map((x) => Kota.fromJson(x))),
      );

  Map<String, dynamic> toJson() => {
        "status": status,
        "query": query.toJson(),
        "kota": List<dynamic>.from(kota.map((x) => x.toJson())),
      };
}

class Kota {
  Kota({
    this.id,
    this.nama,
  });

  String id;
  String nama;

  factory Kota.fromJson(Map<String, dynamic> json) => Kota(
        id: json["id"],
        nama: json["nama"],
      );

  Map<String, dynamic> toJson() => {
        "id": id,
        "nama": nama,
      };
}

class Query {
  Query({
    this.format,
  });

  String format;

  factory Query.fromJson(Map<String, dynamic> json) => Query(
        format: json["format"],
      );

  Map<String, dynamic> toJson() => {
        "format": format,
      };
}
