# Forschungsbericht – Forschungsstufe B.5.7

## Ergebnis

B.5.7 lokalisiert den verbleibenden Fehler auf **kausal mehrdeutige lokale
Kreditzuordnung bei negativer Rückmeldung**. Die bestehende Implementierung
führt im untersuchten Produktionspfad weder erfahrungsübergreifende Eligibility
noch Doppelabschwächungen aus: Bei allen 32.835 beobachteten sensorischen
Kandidatenanwendungen war genau ein Eligibility-Eintrag aktiv.

Die B.5.6-Referenz wurde exakt reproduziert: 236 von 384 Netzen waren robust,
148 nichtrobust, und alle 384 hatten beide Beziehungen erworben. Die
unveränderten B.5.6-Diagnosen wurden gegen die historischen Dateien bestätigt:
57/72 sensorisch reparierbar, 60/72 durch aktuelle Sensorwerte schädigbar,
0 isolierte inhibitorische und 0 isolierte transiente Reparaturen, drei
kombinierte und zwölf offene Fälle.

In 48 präregistriert ausgewählten nichtrobusten Netzen der festen
16er-Discovery und -Bestätigung wurden 9.444 tatsächliche negative sensorische
Gewichtsänderungen rekonstruiert. 7.556 (80,01 %) betrafen eine Synapse, deren
Ablation die aktuelle Motorantwort änderte; 1.888 (19,99 %) waren zeitlich
eligible, aber für diese Antwort nicht notwendig. Alle 48 Netze enthielten
beide Arten. 448 Änderungen (4,74 %) fielen unmittelbar mit einem
Fähigkeitsverlust zusammen. Das trat in 47/48 Netzen auf.

9.376/9.444 negativen Änderungen (99,28 %) folgten einer fehlenden
Motorantwort; 68 (0,72 %) einer falschen eindeutigen Antwort. Im 12er-Raster
traten zusätzlich Doppelantworten auf. Positive Rückmeldung wurde nur nach
tatsächlich korrekter Ausgabe vergeben.

## Technische Kausalanalyse

`Brainstem::create_eligibility()` wählt lokale G3-Kandidaten am
postsynaptischen Feuerereignis. `active_phase` verlangt lokale Aktivität, sagt
aber nichts über die Notwendigkeit eines Kandidaten für die spätere Handlung
aus. `process_modulatory_feedback()` sendet nur skalare Polarität über die
konfigurierte neuronale Evaluationsroute. Erst der tatsächlich erreichte lokale
Empfänger verarbeitet seine Eligibility in `apply_local_modulation()`.

Die Implementierung *kann* mehrere aktive Einträge und damit wiederholte
Änderungen derselben Synapse verarbeiten; der neue Regressionstest weist dies
mit zwei Änderungen in einem Modulationskontakt nach. Im natürlichen B.5.7-
Pfad verbraucht die erste Modulation den einzelnen Eintrag jedoch sofort.
Deshalb waren H2 und H3 empirisch null. Diagnosekopien bestätigten dies:
`natural`, `selected_record` und `deduplicated` schwächten identisch;
`remaining_records` und `no_modulation` schwächten nicht.

Die natürlichen Fortsetzungen zeigen zugleich, warum pauschales Unterdrücken
negativer Modulation falsch wäre. Nach einer weiteren Erfahrung waren in den
48 Fest-16-Netzen 40 natürliche beziehungsweise deduplizierte Kopien wieder
gemeinsam korrekt, aber nur vier Kopien ohne oder mit neutraler Modulation. Nach
64 Erfahrungen lagen die Werte bei 27 beziehungsweise 25. Sämtliche 2.592
Fortsetzungsmessungen an den Offsets 1, 4, 8, 16, 32 und 64 waren gültig.

## Zeit- und Kontrollbefunde

| Raster/Variante | robuste Netze | Gesamt |
|---|---:|---:|
| fest 1 | 0 | 384 |
| fest 12 | 0 | 384 |
| fest 16 | 242 | 384 |
| fest 32 | 274 | 384 |
| unregelmäßig | 181 | 384 |
| S2/R2-50 | 87 | 384 |
| S5 deaktiviert | 87 | 384 |
| No-Learning | 0 | 384 |

Alle Zustände waren gültig. Die unabhängige feste-16-Bestätigung mit Seeds
40001–40064 ergab 254/384 robuste Netze; Discovery ergab 242/384. Damit ist H7
stark bestätigt. Die seed-geclusterte Bootstrap-Schätzung beträgt für fest 16
0,630 (95-%-Intervall 0,583–0,677), für fest 32 0,714
(0,661–0,766) und für unregelmäßige Abstände 0,471 (0,417–0,526). H8 bleibt
indirekt: S4 verändert Feuerfolgen und Robustheit, ist aber keine Lösung der
Kreditzuordnung.

## Ressourcen

Der reproduzierbare Gesamtlauf verarbeitete 4.224 Netze und 74.237.312
Ereignisse in rund 45 CPU-Sekunden. Er protokollierte 32.835 lokale
Kandidatenanwendungen und erzeugte 40.536 diagnostische Zustandskopien. Die
maximale Warteschlange enthielt 32 Ereignisse, der größte geschätzte
Runtime-Zustand 25.424 Byte; ungültige oder ausgeschlossene Fälle: 0. Die drei
großen Ereignistabellen und die Fortsetzungen werden deterministisch mit gzip
gespeichert (zusammen unter 1 MiB).

## Hypothesenentscheidung

- **H1 bestätigt:** Rund ein Fünftel negativer Änderungen war für die aktuell
  bewertete Handlung nicht notwendig.
- **H2 im tatsächlichen Pfad verworfen:** 0 erfahrungsübergreifende Anwendungen;
  verbrauchte Einträge überleben die Rückmeldung nicht.
- **H3 im tatsächlichen Pfad verworfen:** 0 Mehrfachänderungen; die technische
  Möglichkeit bleibt durch Regression abgedeckt.
- **H4 bestätigt:** Lokal notwendige Änderungen können unmittelbar eine
  vorhandene Fähigkeit zerstören.
- **H5 bestätigt:** Fehlende Motoraktivität dominiert negative Modulation.
- **H6 gestützt:** Einzel- und kombinierte Ablationen zeigen, dass die Wirkung
  vom Verhältnis sensorischer und inhibitorischer Beiträge abhängt.
- **H7 bestätigt:** Die Robustheit ändert sich drastisch mit dem Zeitraster.
- **H8 gestützt, nicht isoliert:** R2/S4 verändern Eligibility über Feuerfolgen.
- **H9 bestätigt:** Discovery und unabhängige Erfahrungsströme unterscheiden
  sich, ohne dass die Regel geändert wurde.

## Antworten A–N

**A. Welche Ereignisse schwächen erfolgreiche Verbindungen?** Tatsächlich lokal
eingetroffene negative Modulation nach Fehl- und vor allem Nichtantworten
wendet den negativen Schritt auf den einzelnen aktiven G3-/Eligibility-Eintrag
an. Die vollständige Folge von Modulationskontakt bis Gewicht 3→2, 2→1 oder
1→0 ist im Ereignisprotokoll enthalten.

**B. Wie häufig ist die Synapse notwendig?** In 7.556/9.444 Fällen (80,01 %)
änderte ihre Ablation die aktuell bewertete Motorantwort; in 1.888 Fällen
(19,99 %) nicht.

**C. Wie häufig stammen Kandidaten aus früheren Erfahrungen?** 0/32.835
beobachtete Anwendungen, auch im 12er-Raster. Verbrauch nach der Rückmeldung
verhindert hier die zeitliche Übertragung.

**D. Kann Mehrfachabschwächung auftreten?** Die Implementierung kann es und der
gezielte Regressionstest erzeugt 3→2→1 in einem Modulationskontakt. Im
natürlichen Forschungsbestand trat sie 0-mal auf.

**E. Verursacht Mehrfachabschwächung Fähigkeitsverlust?** Dafür gibt es im
natürlichen Bestand keine Fälle und damit keinen positiven Kausalnachweis.

**F. Welche Rolle spielt die Motorantwort?** 9.376/9.444 negativen Änderungen
(99,28 %) folgten einer fehlenden, 68 einer falschen eindeutigen Antwort. Das
12er-Raster erzeugte zusätzlich zwei Änderungen nach Doppelantworten.

**G. Werden nicht nachweislich beteiligte Gewichte geschwächt?** Ja:
1.888/9.444 negative Änderungen waren zeitlich eligible, aber in der
Einzelablation nicht notwendig. Redundanz wird deshalb nicht mit Irrelevanz
gleichgesetzt; kombinierte Kopien bleiben separat protokolliert.

**H. Welche Änderungen helfen notwendigem Umlernen?** 80,01 % beeinflussten
die aktuelle Motorantwort und dürfen daher nicht pauschal unterdrückt werden.
Welche davon langfristig für eine Umweltumkehr notwendig sind, wurde ohne neue
Regel nicht weiter aufgeteilt und bleibt offen.

**I. Entstehen Verluste auch durch positive Konkurrenz?** Ja als zeitliche
Klasse: 3.234 positive sensorische Änderungen fielen in den 48 Fest-16-Netzen
mit einem unmittelbaren Beziehungsverlust zusammen. Da B.5.7 keine einzelne
positive Änderung unterdrückte, ist deren individueller Kausalanteil noch
offen.

**J. Was erklärt die zwölf offenen B.5.6-Fälle?** B.5.7 erklärt die allgemeine
Ereignisklasse der unpräzisen negativen Kreditzuordnung, ordnet die zwölf
historischen Fälle aber nicht vollständig einzeln zu. Sie bleiben ausdrücklich
offen und dürfen nicht nachträglich als H1 klassifiziert werden.

**K. Welche Wechselwirkung besteht mit S4 und Zeit?** S4 verändert
postsynaptische Feuerfolgen und damit Eligibility-Kandidaten indirekt. Die
Robustheit beträgt 0/384 bei Abstand 12, 242/384 bei 16 und 274/384 bei 32;
die lokale Regel selbst blieb identisch.

**L. Ist eine lokale Regeländerung gerechtfertigt?** Nein. Der Fehler ist
lokalisiert, aber keine minimale Alternative erhält notwendige negative
Korrekturen und ist zugleich unabhängig bestätigt. K13/K14 bleiben aus.

**M. Bleiben Beziehungen und Umlernen erhalten?** Ja für den integrierten
Stand: Der Produktionspfad ist bitgenau unverändert, E1/E2, Retention,
Umkehrreferenz, geschützte Exploration, Persistenz und deterministische
Fortsetzung bleiben erhalten. Es wurde keine neue Variante freigegeben.

**N. Was soll B.5.8 untersuchen?** Eine streng präregistrierte, ausschließlich
lokale Unterscheidung zwischen zeitlicher Eligibility und kausal notwendigem
negativem Beitrag – einschließlich positiver Konkurrenz und der zwölf offenen
B.5.6-Fälle, ohne Zielwissen oder Schutz früher erfolgreicher Gewichte.

## Merge-Empfehlung

**B.5.7 nach `main` mergen: Ja.** K13 und K14 bleiben deaktiviert. Es wird
keine neue Lernfunktion freigegeben.
