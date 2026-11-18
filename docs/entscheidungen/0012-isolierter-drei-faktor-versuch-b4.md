# Entscheidung 0012: Isolierter Drei-Faktor-Versuch B.4

- Status: angenommen für Forschung, nicht für Produktion
- Datum: 18. November 2026
- Bezug: Entscheidungen 0010 und 0011

## Kontext

B.3.1 konnte eine zuverlässige Handlungsidentität für verzögerte Rückmeldungen
nicht nachweisen. Entscheidung 0011 bleibt deshalb gültig. Der Auftrag B.4
erlaubt dennoch einen engeren Versuch: lokale Aktivitätsspuren dürfen zusammen
mit einem später eintreffenden, skalaren Modulationssignal Gewichte verändern.

## Entscheidung

EVE erhält einen standardmäßig deaktivierten Drei-Faktor-Pfad. Eine Änderung
ist nur zulässig, wenn am empfangenden Neuron eine begrenzte lokale Spur liegt
und ein neuronaler Modulationsimpuls dort eintrifft. Die Rückmeldung enthält
weder Aktionskennung noch Synapsenliste. Positive Änderungen sind durch
Gewichtsobergrenze und Eingangsbudget begrenzt; negative enden bei null.
Rückmeldungsursprüngliche Spuren sind von Änderungen ausgeschlossen.

Die B.3.1-Kontextsignaturen sind im Primärversuch deaktiviert. B.4 ist keine
Freigabe für allgemeines Online-Lernen.

## Folgen

- Persistenzformat 9 speichert die neue Konfiguration, Metriken und ausstehende
  Modulationsereignisse.
- K0–K15 und 64 gepaarte Seeds prüfen Wirkung und Fehlzuordnung.
- Ein Übergang zur nächsten Stufe setzt einen Verhaltensvorteil gegenüber K0,
  Generalisierung und Umkehrlernen voraus. Reine Gewichtsänderung genügt nicht.

## Ergebnis

Der Mechanismus arbeitet technisch lokal und begrenzt, bestand aber das
Verhaltenskriterium nicht. E2 war im Mittel nicht besser als K0; Umkehrlernen
scheiterte. Die Produktionssperre bleibt bestehen.
