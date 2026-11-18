# Forschungsstufe B – Lokales neuronales Ereignisgedächtnis

## Kurzfassung

Forschungsstufe B ergänzt jedes Neuron optional um einen kompakten Ringspeicher
der zuletzt empfangenen Synapsen-IDs. Die Primärmatrix umfasst 640 Läufe: 64
vollständig dokumentierte Seeds, fünf Speichergrößen G0 bis G4 und jeweils die
aktive M3-Schaltung sowie ihre identische, funktional unterbrochene K2-Variante.
Lernen und Wachstum blieben deaktiviert; die Refraktärregel blieb unverändert.

In der erweiterten Stichprobe entstanden bei aktiver M3-Schaltung 38 Läufe mit
Datenmotoraktivität und 14 technisch nachgewiesene M3-Abschlüsse. K2 erzeugte
bei gleicher Datenaktivität keinen nachgewiesenen Schaltungsabschluss.

G2 mit vier Einträgen war die kleinste Variante, die alle 14 nachgewiesenen
M3-Wege am Feuerentscheid vollständig lokal rekonstruierte. G1 mit zwei
Einträgen behielt den unmittelbaren Abschlusseingang, verlor aber immer
mindestens einen der drei für den Akkumulator notwendigen Eingänge. G3 und G4
lieferten am Feuerentscheid keinen zusätzlichen Rekonstruktionserfolg gegenüber
G2.

Die spätere Verfügbarkeit unterscheidet die Varianten deutlicher. Nach drei
zusätzlichen Eingangsaktivierungen enthielt G1 nur noch in 4 von 14
erfolgreichen Akkumulatorfällen mindestens eine relevante Synapsen-ID. G2
enthielt sie zunächst in 14 von 14 und nach Eigenrückmeldung in 13 von 14, G3
und G4 jeweils in 14 von 14. Da der Ring nur IDs speichert, ist allerdings
nicht entscheidbar, ob eine nachträglich sichtbare gleiche ID noch das
ursprüngliche Ereignis oder eine neue Aktivierung derselben Synapse bezeichnet.

Gezielte K4-Unterbrechungen änderten 10 von 14 zuvor erfolgreichen Abschlüssen.
Vier Wege schlossen trotz der einzelnen Unterbrechung über weitere Eingänge
weiterhin ab. Dieses Ergebnis bestätigt Beteiligung, zeigt aber zugleich die
erwartete Mehrfach- und Ersatzwirkung von Eingängen.

G3 bietet den sinnvollsten untersuchten Kompromiss für eine spätere verzögerte
Rückmeldungszuordnung. G2 genügt für die unmittelbare M3-Rekonstruktion, G3 ist
gegen wenige nachfolgende Eingänge robuster, und G4 zeigte in dieser Stichprobe
keinen praktischen Zusatznutzen. Ein reiner Ringpuffer reicht dennoch nicht für
gezieltes Lernen: Er markiert weder die konkrete Ereignisinstanz noch den
beitragenden Anteil und transportiert keine Bewertung rückwärts durch die
Handlungskette.

## A. Technische Umsetzung

### A.1 Lokaler Ringspeicher

Der Speicher verwendet zusammenhängende indexbasierte Arrays. Jeder Eintrag
enthält ausschließlich eine vier Byte große `SynapseId`. Pro aktivem Neuron
werden Ringposition, Belegung und Überschreibungszähler separat geführt.

Ein Impuls wird vor Lernprüfung, Membranintegration und Feuerentscheidung
eingetragen. Deshalb erfasst der Speicher auch Nullgewicht-Eingänge,
Wiederholungen und Eingänge während der Refraktärphase. Reihenfolge und
Wiederholungen bleiben erhalten; bei Überlauf wird der älteste Eintrag ersetzt.

G0 allokiert weder Slots noch Verwaltungsarrays. Damit bleiben Verarbeitung,
Zähler und Zustandsfolge bei deaktiviertem Ereignisgedächtnis unverändert.

### A.2 Feuer-Momentaufnahme

Die optionale technische Impulsbeobachtung kopiert den lokalen Ring unmittelbar
nach der Feuerentscheidung. Der auslösende Impuls ist zu diesem Zeitpunkt der
letzte Eintrag. Später verarbeitete Warteschlangenereignisse können die Kopie
nicht verändern. Ohne Beobachter entsteht keine solche Kopie.

Die Momentaufnahme ist Forschungsdiagnose und kein zusätzlicher neuronaler
Zustand. Das Stammhirn übergibt dem Neuron keine Kausalitätskette.

### A.3 Persistenz und Lebensdauer

Der lokale Ring bleibt über Feuer-, Aktivitätsphasen-, Antwort- und
Refraktärwechsel bestehen, bis neue Eingänge Einträge überschreiben. Er wird
mit dem neuronalen Zustand gespeichert und wiederhergestellt. Persistenzformat
4 bewahrt logische Reihenfolge und Überschreibungszähler; Formate 1 bis 3
bleiben abwärtskompatibel lesbar und erhalten G0.

### A.4 Handlungsgebundene Rekonstruktion

Eine erste Auswertung zählte lokale M3-Aktivität auch außerhalb der konkreten
Antwortphase. Dieser methodische Fehler wurde durch die unabhängige Diagnose
sichtbar. Die endgültige lokale Auswertung akzeptiert deshalb nur
Feuerentscheidungen zwischen dem ersten Datenmotorereignis und dem
Abschlussereignis derselben Antwort.

Für einen lokalen M3-Weg müssen:

1. mindestens drei aktive Daten-zu-Akkumulator-Einträge in der
   Akkumulator-Momentaufnahme erhalten sein und
2. der aktive Akkumulator-zu-Abschluss-Eingang in der Abschluss-Momentaufnahme
   vorhanden sein.

Die technische Diagnose rekonstruiert weiterhin unabhängig die tatsächlich
verarbeiteten Impulse und Feuerereignisse.

## B. Versuchsaufbau

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| reguläre Startsynapsen | 1.024 |
| Geometrie | 8 × 8 × 4 |
| reguläres Startgewicht | 0 |
| angeborene Grundverschaltung | V2, Gewicht 3 |
| Motorschaltung | M3 |
| Akkumulatorgewicht | 1 |
| Abschlussgewicht | 3 |
| Feuergrenze | 3 |
| Refraktärereignisse | 16 |
| Lernen und Wachstum | deaktiviert |
| Eingabe | `AB` |
| Exploration | E4-neu, 16 Aktivierungen |
| Ereignisbudget | 100.000 |
| Seeds | 64, davon alle 32 aus A.2 |

Die Varianten sind G0 = 0, G1 = 2, G2 = 4, G3 = 8 und G4 = 16 Einträge je
Neuron. Jeder Seed wurde aus einem neu initialisierten Zustand mit aktiver M3-
Schaltung und mit identischer K2-Topologie bei Gewicht 0 ausgeführt.

Der Runner erzeugt sieben CSV-Dateien mit insgesamt 5.830 Zeilen:

- Parameter,
- 640 Primärläufe,
- 2.850 Feuer-Momentaufnahmen,
- 1.948 Haltbarkeitsmessungen,
- 14 gezielte K4-Vergleiche,
- 288 Wiederholungsläufe,
- 60 Performanceläufe.

## C. Primärergebnisse G0 bis G4

Die Datenmotoraktivität war über alle Varianten identisch: 38 von 64 aktive
Läufe. Die aktive M3-Schaltung erzeugte 14 technisch nachgewiesene Wege; K2
erzeugte keinen. Die zusätzlichen erfolgreichen Seeds gegenüber A.2 waren
107, 157, 173, 179, 233, 239 und 241. Alle erfolglosen Seeds blieben in der
Auswertung.

| Variante | Kapazität | technische M3-Wege | lokal vollständige Wege | Übereinstimmung in 64 K1-Läufen |
|---|---:|---:|---:|---:|
| G0 | 0 | 14 | 0 | 50 |
| G1 | 2 | 14 | 0 | 50 |
| G2 | 4 | 14 | 14 | 64 |
| G3 | 8 | 14 | 14 | 64 |
| G4 | 16 | 14 | 14 | 64 |

G0 und G1 stimmen bei den 14 Erfolgen erwartungsgemäß nicht mit der Diagnose
überein. G1 enthält am Akkumulator höchstens zwei der drei notwendigen
Ereignisse. Der Abschlusseingang selbst bleibt erhalten, reicht aber nicht zur
Rekonstruktion der vorausgegangenen Akkumulation.

In allen 64 K2-Läufen stimmten lokale und technische Auswertung überein: kein
vollständiger Weg. Nullgewicht-Eingänge wurden trotzdem lokal gespeichert,
führten aber weder Akkumulator- noch Abschlussfeuer im Handlungsfenster herbei.

## D. Beteiligung und K3-Kontrolle

Die erfolgreichen Momentaufnahmen enthielten neben M3-Eingängen auch reguläre
Nullgewicht-Synapsen:

| Variante | erfolgreiche Wege | Wege mit nicht beitragendem Eintrag | solche Einträge |
|---|---:|---:|---:|
| G1 | 14 | 6 | 6 |
| G2 | 14 | 7 | 9 |
| G3 | 14 | 7 | 11 |
| G4 | 14 | 7 | 11 |

Die Zahl steigt mit der Kapazität, ohne die vollständige Rekonstruktion ab G2
zu verhindern. Das bestätigt den zentralen Vorbehalt: zeitliche Nähe im Ring
ist keine Aussage über erregende Wirkung. Die technische Gewichtsinformation
klassifiziert diese Einträge als registriert, aber nicht beitragend; diese
Klassifikation wird nicht in den neuronalen Ring geschrieben.

## E. K4 – gezielte Einzelunterbrechung

Für jeden der 14 nachgewiesenen G3-Wege wurde aus der Akkumulator-
Momentaufnahme eine aktive beteiligte Synapse gewählt. Ein zweiter Lauf begann
aus identischem Seed und identischer Konfiguration; nur das Gewicht dieser
Synapse wurde auf 0 gesetzt.

| Ergebnis | Anzahl |
|---|---:|
| Ausgangswege technisch und lokal vollständig | 14 |
| Abschluss nach Unterbrechung entfallen | 10 |
| Abschluss weiterhin technisch und lokal vollständig | 4 |

Die zehn veränderten Ausgänge liefern kontrollierte Evidenz, dass die gewählte
Verbindung für diesen Verlauf notwendig war. Die vier unveränderten Ausgänge
widerlegen ihre Beteiligung nicht: weitere Datenimpulse konnten die
Akkumulatorschwelle weiterhin erreichen. Kein Ergebnis wird als Beweis für die
alleinige Ursache der gesamten Handlung interpretiert.

## F. Haltbarkeit bis zur Rückmeldung

Die folgende Tabelle betrachtet nur die 14 technisch nachgewiesenen Wege und
fragt, in wie vielen Akkumulatorringen mindestens eine M3-Synapsen-ID sichtbar
war:

| Variante | am Feuerentscheid | nach Abschluss | nach 3 zusätzlichen Eingangsaktivierungen | nach Eigenrückmeldung |
|---|---:|---:|---:|---:|
| G1 | 14/14 | 14/14 | 4/14 | 4/14 |
| G2 | 14/14 | 14/14 | 14/14 | 13/14 |
| G3 | 14/14 | 14/14 | 14/14 | 14/14 |
| G4 | 14/14 | 14/14 | 14/14 | 14/14 |

Der Abschlussring behielt seinen relevanten Eingang in allen Varianten, weil
in dieser Messung keine weiteren Abschlussimpulse eintrafen. Der Akkumulator
war der belastete Speicherort.

Die Tabelle misst sichtbare IDs, nicht die Identität einzelner
Ereignisinstanzen. Nachfolgende Aktivität derselben Synapse kann einen alten
Eintrag ersetzen und dennoch gleich aussehen. Der Ring ist daher eine
brauchbare lokale Kandidatenliste, aber kein Nachweis, dass exakt das
ursprüngliche Ereignis bis zur Rückmeldung erhalten blieb.

## G. Wiederhergestellte und fortgeführte Zustände

Sechs etablierte Seeds wurden für G0, G2 und G3 jeweils achtmal aus
wiederhergestelltem und fortgeführtem Zustand ausgeführt.

| Zustand | Variante | Antwortphasen | mit Daten | technische M3-Wege | lokale M3-Wege |
|---|---|---:|---:|---:|---:|
| wiederhergestellt | G0 | 48 | 24 | 16 | 0 |
| wiederhergestellt | G2 | 48 | 24 | 16 | 16 |
| wiederhergestellt | G3 | 48 | 24 | 16 | 16 |
| fortgeführt | G0 | 48 | 3 | 2 | 0 |
| fortgeführt | G2 | 48 | 3 | 2 | 2 |
| fortgeführt | G3 | 48 | 3 | 2 | 2 |

Fortgeführte Läufe verzeichneten über die jeweilige Matrix 820 refraktär
unterdrückte Feuerentscheidungen, wiederhergestellte Läufe 64. Weder
Refraktärzeit noch Membranpotenzial wurden zurückgesetzt. Der Ringspeicher
behebt die aus A.2 bekannte Wiederholungsinstabilität nicht und soll das auch
nicht; er dokumentiert die wenigen tatsächlich entstehenden Handlungen korrekt.

## H. Speicher und Performance

| Variante | ID-Speicher | Verwaltung | zusätzlicher lokaler Speicher | mittlerer Laufzeitspeicher |
|---|---:|---:|---:|---:|
| G0 | 0 KiB | 0 KiB | 0 KiB | 60.595 B |
| G1 | 2 KiB | 4 KiB | 6 KiB | 66.739 B |
| G2 | 4 KiB | 4 KiB | 8 KiB | 68.787 B |
| G3 | 8 KiB | 4 KiB | 12 KiB | 72.883 B |
| G4 | 16 KiB | 4 KiB | 20 KiB | 81.075 B |

Für eine Million Neuronen ergeben sich einschließlich Verwaltung 24 MB (G1),
32 MB (G2), 48 MB (G3) und 80 MB (G4). Die Werte verwenden dezimale MB.

Die sechs Läufe je Variante dauerten ohne Detaildiagnose im Mittel 49,5 bis
52,1 Mikrosekunden. Bei dieser kurzen Dauer war kein belastbarer
Ringspeicher-Zeitaufschlag gegenüber G0 auflösbar; Streuung dominierte das
Signal. Mit Diagnose und Momentaufnahmekopien lagen G1 bis G4 bei 55,7 bis
57,7 Mikrosekunden gegenüber 52,2 Mikrosekunden für G0. Dieser externe
Diagnoseaufwand ist nicht Teil der neuronalen Verarbeitung.

Die Aufnahme eines Impulses benötigt keinen Netzdurchlauf und keine dynamische
Allokation. Nur das technische Kopieren einer Feuer-Momentaufnahme ist linear
in der kleinen konfigurierten Ringgröße.

## I. Tests und Reproduzierbarkeit

Alle 86 automatisierten Tests bestehen. Die 14 neuen Prüfungen decken ab:

- Einzelaufnahme, Reihenfolge und Wiederholung,
- Ringüberlauf und Überschreibungszählung,
- Nullgewicht-Eingänge,
- unveränderliche Momentaufnahme am Feuerentscheid,
- Verarbeitung während Refraktärität,
- einfache Feuer- und vollständige M3-Rekonstruktion,
- nicht zuordenbare Einträge,
- unterbrochene M3-Wege,
- identische Seeds,
- Persistenz und fortgesetzte Ausführung,
- G0-Referenzverhalten,
- gezielte Einzelunterbrechung.

Identische Seeds erzeugen dieselben neuronalen Ereignisse, Antworten,
Zustandsfingerabdrücke und lokalen Rekonstruktionen. Nur Laufzeit- und
Durchsatzfelder sind maschinen- und lastabhängig.

## J. Antworten auf die Abschlussfragen

1. **Können Neuronen Eingänge lokal und geordnet speichern?** Ja, einschließlich
   Wiederholungen, Nullgewicht und Refraktärphase.
2. **Was ist beim eigenen Feuern verfügbar?** Der aktuelle geordnete Ring
   einschließlich des auslösenden letzten Eingangs; kleine Ringe verlieren
   ältere Summanden.
3. **Werden beteiligte Eingänge erfasst?** Ja, ab G2 alle für die untersuchten
   M3-Feuerentscheidungen notwendigen IDs; zugleich erscheinen nicht
   beitragende Eingänge.
4. **Lässt sich M3 rekonstruieren?** Mit G2 bis G4 alle 14 technisch
   nachgewiesenen Wege und keine K2-Wege.
5. **Welche Größe ist der beste Kompromiss?** G3. G2 reicht unmittelbar, G3
   ist bei verzögerter Auswertung robuster, G4 brachte keinen Zusatznutzen.
6. **Was geht bis zur Rückmeldung verloren?** G1 verliert den Großteil der
   Akkumulatorspur; G2 kann sie vollständig verlieren. Auch G3/G4 können die
   ursprüngliche Ereignisinstanz nicht von späterer gleicher Aktivität
   unterscheiden.
7. **Was fehlt für spätere Anpassung?** Mindestens eine lokale Markierung der
   konkreten beitragenden Feuerentscheidung mit begrenzter Lebensdauer sowie
   ein experimentell kontrollierter Weg, Rückmeldung an solche Markierungen
   zu binden. Der reine Ring darf nicht selbst als Kausalitätsbeweis gelten.

## Schlussfolgerung

Das lokale Ereignisgedächtnis enthält tatsächlich relevante neuronale
Beteiligungsinformation. Es ist mehr als ein abweichendes Protokoll: In G2 bis
G4 stimmt die aus lokal verfügbaren IDs rekonstruierte M3-Handlung in allen 64
aktiven Primärfällen mit der unabhängigen Diagnose überein, und gezielte
Unterbrechungen verändern einen Großteil der zuvor erfolgreichen Verläufe.

G3 wird als Forschungsgrundlage empfohlen, nicht als endgültige
Lernarchitektur. Vor automatischer Gewichtsänderung müssen Ereignisinstanz,
Beitragsunsicherheit, Rückmeldungsweg und Wiederholungsinstabilität ausdrücklich
behandelt werden.
