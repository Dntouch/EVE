# EVE – Lokales neuronales Ereignisgedächtnis

## Zweck und Abgrenzung

Das lokale Ereignisgedächtnis hält je Neuron eine begrenzte, geordnete Folge
der zuletzt empfangenen Synapsen-IDs fest. Es beantwortet ausschließlich die
Frage, **welche lokalen Eingänge zuletzt eintrafen**. Es enthält keine
Bewertung, keinen Zeitstempel, keine fertige Kausalitätskette und löst keine
Gewichtsänderung aus.

Die technische Ereignisdiagnose bleibt davon getrennt. Sie darf Kopien des
lokalen Speichers für die Forschung auslesen, gehört aber nicht zum
neuronalen Zustand.

## Datenmodell

Die experimentelle Kapazität ist für einen Lauf global konfiguriert und kann
0, 2, 4, 8 oder 16 Einträge pro Neuron betragen. Ein Eintrag ist eine
vier Byte große `SynapseId`.

Die Umsetzung verwendet vier zusammenhängende Arrays:

- `local_event_slots_`: alle Ringspeichereinträge,
- `local_event_write_positions_`: nächste Schreibposition pro Neuron,
- `local_event_sizes_`: aktuell belegte Einträge pro Neuron,
- `local_event_overwrites_`: kumulierte Überschreibungen pro Neuron.

Die Einträge eines Neurons liegen in einem festen Abschnitt des Slot-Arrays.
Damit benötigt ein Eingang weder Speicherallokation noch einen Durchlauf über
das Netzwerk. Eine Aufnahme besteht aus einem indexierten Schreibzugriff und
der Aktualisierung des Ringzeigers.

Bei Kapazität 0 werden weder Slot- noch Verwaltungsarrays angelegt. G0 behält
damit die bisherige neuronale Verarbeitung und trägt keinen zusätzlichen
lokalen Speicher.

## Aufnahmeregel

Jeder tatsächlich verarbeitete synaptische Eingangsimpuls wird vor
Lernprüfung, Membranintegration und Feuerentscheidung eingetragen. Das gilt
auch für:

- Synapsen mit Gewicht 0,
- wiederholte Impulse derselben Synapse,
- Eingänge während einer Refraktärphase,
- Eingänge, die allein kein Feuern auslösen.

Wiederholungen bleiben als getrennte Ringpositionen sichtbar. Ist der Ring
voll, überschreibt der nächste Eingang den ältesten Eintrag. Ausgelesen wird
immer in der logischen Reihenfolge vom ältesten zum neuesten erhaltenen
Eintrag.

## Momentaufnahme am Feuerentscheid

Der Eingangsimpuls, der die Schwelle erreicht, ist bereits der letzte Eintrag
des Rings, wenn die Feuerentscheidung fällt. Ist ein Forschungsbeobachter
aktiv, kopiert die Laufzeit den Ring unmittelbar nach dieser Entscheidung in
die `ImpulseObservation`.

Diese Kopie gehört zum technischen Versuchsprotokoll. Sie bleibt unverändert,
auch wenn das daraus entstandene Feuerereignis erst später aus der
Ereigniswarteschlange verarbeitet wird und inzwischen weitere Impulse
eintreffen. Ohne Beobachter wird keine Momentaufnahme erzeugt.

## Lebensdauer und Persistenz

Der Ringspeicher ist neuronaler Zustand:

- Netzinitialisierung leert ihn.
- Feuer-, Aktivitätsphasen- und Refraktärwechsel leeren ihn nicht.
- Neue Eingänge verdrängen alte Einträge ausschließlich durch Ringüberlauf.
- Speichern und Wiederherstellen der Laufzeit erhält Reihenfolge und
  Überschreibungszähler.

Persistenzformat 4 speichert die logische Ereignisfolge je Neuron. Beim Laden
wird daraus eine kanonische Ringbelegung rekonstruiert. Zustände der Formate 1
bis 3 bleiben lesbar und erhalten ein deaktiviertes, leeres Ereignisgedächtnis.

## Lokale und technische Kausalität

Eine lokale Spur kann Beteiligung anzeigen, beweist aber nicht allein
Kausalität:

- Ein positiver Eingang kann zum Membranpotenzial beigetragen haben.
- Ein Nullgewicht-Eingang wurde nur registriert.
- Ein relevanter Eingang kann durch spätere Eingänge verdrängt worden sein.
- Dieselbe Synapsen-ID kann vor und nach einer Handlung erneut auftreten.
- Der letzte Eingang löst die Entscheidung aus, ist aber nicht zwingend der
  einzige beteiligte Eingang.

Für M3 wertet die lokale Rekonstruktion deshalb nur Feuerentscheidungen im
Fenster zwischen dem ersten Datenmotorereignis und dem Abschluss der konkreten
Antwort aus. Die unabhängige technische Diagnose verfolgt weiterhin reale
Impulse und Feuerereignisse. Erst der Vergleich beider Ergebnisse erlaubt
eine belastbare Aussage.

## Speicherbedarf

Für 256 Neuronen entstehen bei aktivem Speicher 16 Byte Verwaltungsdaten pro
Neuron, insgesamt 4 KiB. Hinzu kommen die ID-Einträge:

| Variante | Einträge | ID-Speicher | Verwaltung | Summe lokal |
|---|---:|---:|---:|---:|
| G0 | 0 | 0 KiB | 0 KiB | 0 KiB |
| G1 | 2 | 2 KiB | 4 KiB | 6 KiB |
| G2 | 4 | 4 KiB | 4 KiB | 8 KiB |
| G3 | 8 | 8 KiB | 4 KiB | 12 KiB |
| G4 | 16 | 16 KiB | 4 KiB | 20 KiB |

Bei einer Million Neuronen entspricht das einschließlich Verwaltung ungefähr
24, 32, 48 beziehungsweise 80 MB für G1 bis G4.

## Bewusste Grenzen

Der Speicher unterscheidet keine Ereignisinstanzen derselben Synapse über den
Ringinhalt hinaus. Er markiert weder beitragende Synapsen noch verteilt er
Handlungserfolg rückwärts durch das Netz. Eligibility Traces, Belohnung,
Abschwächung, STDP und neue Refraktärregeln bleiben Folgestufen vorbehalten.
