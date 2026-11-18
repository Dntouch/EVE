# EVE – Lokale neuronale Lernberechtigung

## Verantwortung und Bedeutung

Das Stammhirn definiert Erzeugung, Auswahl, Gültigkeit und Verfall einer
Eligibility-Markierung. Das neuronale Netzwerk speichert den Zustand. Das BIOS
plant Ablaufereignisse und stellt ausschließlich technische Diagnoseansichten
bereit.

Eine Markierung bedeutet: *Diese Eingangssynapse war in der lokalen
Momentaufnahme einer Feuerentscheidung enthalten und ist ein möglicher
Lernkandidat.* Sie bedeutet nicht, dass die Synapse notwendig, hilfreich,
schädlich oder ursächlich war.

## Erzeugung bei der Feuerentscheidung

Ein eingehender Impuls wird zuerst unverändert in G3 eingetragen. Löst er eine
Feuerentscheidung aus, wählt das Stammhirn sofort Kandidaten aus der nun
vorliegenden lokalen Ringreihenfolge. Die Auswahl wird kopiert und erhält eine
lokale 64-Bit-Generation. Das anschließend in die Warteschlange gestellte
`fire`-Ereignis kann diese Momentaufnahme nicht mehr verändern.

L1 übernimmt die jüngsten G3-Einträge. L2 filtert Einträge, deren aktuelles
Gewicht nicht positiv ist. L2 ist eine Versuchsregel; insbesondere beweist ein
positives Gewicht keine Notwendigkeit für das Feuern.

Bei `unique` wird vom neuesten Eintrag rückwärts ausgewählt, jede Synapsen-ID
nur einmal aufgenommen und die Ergebnisliste wieder chronologisch geordnet.
Bei `repeated` bleiben Mehrfachaktivierungen bis zur Kapazitätsgrenze erhalten.
Die jüngsten Kandidaten haben Vorrang.

## Kompakte Speicherung

Der Zustand liegt als indexbasierte Structure-of-Arrays im Netzwerk:

- feste Kandidatenplätze aus `SynapseId`,
- Größe, Generation und Ablaufzeit je Entscheidungsplatz,
- Schreibposition, nächste Generation und Überschreibungszahl je Neuron.

Es gibt keine Heap-Allokation pro neuronaler Markierung und keinen Zeitstempel
pro Synapse. Die logische `EligibilityRecord`-Ansicht wird nur beim Auslesen
gebildet.

Mehrere Feuerentscheidungen unterstützen drei Regeln:

| Regel | Wirkung |
|---|---|
| `overwrite` | verwirft alle älteren Entscheidungsmarkierungen |
| `retain` | behält eine begrenzte Zahl getrennter Generationen |
| `merge_unique` | vereinigt Kandidaten in eine neue Generation |

Überschreiben und Zusammenführen machen die alten Ablaufereignisse veraltet;
diese werden anhand ihrer Generation wirkungslos verworfen.

## Ereignisgesteuerter Verfall

Nach jeder erzeugten Markierung plant das BIOS ein
`eligibility_expiry`-Ereignis für `Entscheidungszeit + Dauer`. Das Ereignis
adressiert genau ein Neuron und genau eine Generation. Es durchsucht weder das
Netzwerk noch Synapsen außerhalb der kleinen lokalen Entscheidungsplätze.

D0 wird während der Impulsbeobachtung sichtbar. Das eigentliche gleichzeitige
Feuerereignis wird vor seinem Ablauf verarbeitet; nach Abschluss aller
gleichzeitigen Ereignisse ist D0 nicht mehr aktiv.

Bei einem technischen Prüfsignal exakt zum Ablaufzeitpunkt gilt verbindlich:
Alle bereits geplanten Ablaufereignisse dieses Zeitpunkts werden zuerst nach
Zeit und Einfügereihenfolge verarbeitet, anschließend liest der Prüfer den
Zustand. Die Markierung ist bei diesem Prüfsignal daher nicht mehr gültig.

## Unabhängigkeit von Erholung und Lernen

R1/R2F-Erholung und Eligibility besitzen getrennte Generationen und Ereignisse.
Erholung löscht keine gültige Markierung. Eligibility-Ablauf verändert nur den
betroffenen Eligibility-Platz. Insbesondere bleiben unverändert:

- Membranpotenzial und Refraktärzustand,
- G3-Inhalt und G3-Überschreibungszähler,
- Synapsengewicht, Endpunkte und Aktivitätszähler,
- Feuer- und Impulsereignisse.

Die B.2-Metriken zählen Entscheidungen, Kandidaten, positive und
Nullgewicht-Kandidaten, Überschreibungen sowie geplante, verarbeitete und
veraltete Ablaufereignisse. Keine dieser Diagnosen fließt in die Markierung
zurück.

## Persistenz

Format 6 speichert Konfiguration, alle aktiven Entscheidungsmarkierungen,
lokale Generations- und Überschreibungszähler sowie ausstehende
Ablaufereignisse. Formate 1 bis 5 werden mit deaktiviertem Eligibility-Zustand
weiterhin geladen. Nach Wiederherstellung kann ein veraltetes Ereignis eine
ersetzte Generation nicht löschen.

## Ressourcen

Die Standardkonfiguration mit zwei Entscheidungen und je acht reservierten
Kandidaten benötigt in der aktuellen SoA-Auslegung 124 Byte pro Neuron. Darin
enthalten sind Kandidatenplätze und Verwaltungsfelder; tatsächlich aktive
Kandidaten benötigen keine zusätzliche Einzelallokation. Die Schätzung beträgt
12,4 MB für 100.000 und 124 MB für eine Million Neuronen.

Die vorhandene sortierte Ereigniswarteschlange bleibt der Skalierungsvorbehalt:
Einfügen ist linear in der Zahl ausstehender Ereignisse. Der lokale
Eligibility-Zugriff selbst ist durch die konfigurierten Kapazitäten begrenzt.

