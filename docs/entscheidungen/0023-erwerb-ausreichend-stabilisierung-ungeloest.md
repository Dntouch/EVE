# Entscheidung 0023: Erwerb ist ausreichend, Stabilisierung bleibt ungelöst

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0022

## Kontext

B.5.1 verfolgt 1.536 Sensorpfade in 768 Netzen über je 512 natürlich
gemischte Episoden. Alle Pfade erhalten wiederholt reale positive Erfahrung,
überschreiten die Wirksamkeitsschwelle und werden mindestens einmal autonom
korrekt. Die validierte B.5.0-Wiederholung bleibt unverändert: Nach zwei
positiven Erfahrungen sind 364/384, nach drei 384/384 Diagnosekopien korrekt.

Alle natürlich erworbenen Pfade werden später auch wieder verloren und
wiedererworben. Nach 64 weiteren Erfahrungen sind nur 43,88 % der
Entdeckungs- und 45,44 % der Bestätigungspfade korrekt. Beide Beziehungen
sind am Ende nur in 140/768 Netzen gleichzeitig verfügbar. Inhibitorische
Änderungen dominieren den ersten Verlust, erklären ihn durch isolierte
Rücknahme aber nur teilweise. Sensoränderungen und transiente Zustände tragen
eigenständig bei.

## Entscheidung

Die vorhandene lokale Plastizität gilt als grundsätzlich ausreichend für
momentanen Erwerb und Wiedererwerb. Sie gilt nicht als ausreichend für stabile
gleichzeitige Verfügbarkeit mehrerer Beziehungen.

Es wird kein neuer K9-Mechanismus implementiert oder freigegeben. Insbesondere
werden Lernschritt, Feuergrenze, Eligibility-Dauer, Explorationsstrategie,
inhibitorische Lernrichtung und automatischer Gewichtsschutz nicht durch
Parameter-Tuning verändert. Das bloße Festhalten des erworbenen Zielgewichts
verschlechtert in der Diagnose die gleichzeitige Retention und ist keine
Lösung.

Als gesichert gelten:

- natürliche positive Wiederholung ist vorhanden und nicht der Hauptengpass,
- Ziel 1→2 kann Erwerb kausal notwendig und hinreichend erzeugen,
- Konkurrenz 2→1 kann ebenfalls eine eindeutige Fähigkeit freilegen,
- Erwerb und stabile Speicherung sind getrennte Eigenschaften,
- inhibitorische Änderungen sind der häufigste frühe Verlustbegleiter,
- einzelne Sensor- oder Hemmrollbacks erklären nur einen Teil der Verluste,
- Fähigkeiten können ohne Gewichtsänderung vorübergehend verschwinden,
- häufigere Erfahrung einer Beziehung kann die andere Beziehung schädigen,
- unabhängige Bestätigung reproduziert den instabilen Lebenszyklus.

## Entscheidungsvorlage für B.5.2

B.5.2 soll die lokale Stabilisierung konkurrierender Bindungen untersuchen.
Vor einer Mechanismusänderung sind mindestens drei Ursachen getrennt zu
isolieren:

1. inhibitorische Eligibility für Beiträge, die an der bewerteten Handlung
   nicht kausal beteiligt waren,
2. gemeinsame sensorische Kandidaten, deren Änderung eine andere Beziehung
   trifft,
3. transiente Membran- und Erholungszustände, die den Abruf ohne
   Gewichtsänderung verändern.

Eine Forschungsvariante ist erst zulässig, wenn eine dieser Ursachen in
kontrollgleichen Kopien notwendig nachgewiesen wird und ihre minimale lokale
Korrektur sowohl Erwerb als auch die andere Beziehung in unabhängigen Seeds
erhält.
