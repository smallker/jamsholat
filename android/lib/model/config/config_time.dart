import 'dart:convert';

class ConfigTime {
  ConfigTime({
    this.config,
    this.hour,
    this.minute,
    this.day,
    this.month,
    this.year,
  });

  int config;
  int hour;
  int minute;
  int day;
  int month;
  int year;

  factory ConfigTime.fromJson(String str) =>
      ConfigTime.fromMap(json.decode(str));

  String toJson() => json.encode(toMap());

  factory ConfigTime.fromMap(Map<String, dynamic> json) => ConfigTime(
        config: json["config"] == null ? null : json["config"],
        hour: json["hour"] == null ? null : json["hour"],
        minute: json["minute"] == null ? null : json["minute"],
        day: json["day"] == null ? null : json["day"],
        month: json["month"] == null ? null : json["month"],
        year: json["year"] == null ? null : json["year"],
      );

  Map<String, dynamic> toMap() => {
        "config": config == null ? null : config,
        "hour": hour == null ? null : hour,
        "minute": minute == null ? null : minute,
        "day": day == null ? null : day,
        "month": month == null ? null : month,
        "year": year == null ? null : year,
      };
  factory ConfigTime.fromDateTime(DateTime date) => ConfigTime(
      config: 1,
      minute: date.minute,
      hour: date.hour,
      day: date.day,
      month: date.month,
      year: date.year);
}
