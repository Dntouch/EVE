# EVE – Ereignisgesteuerte Zeit und neuronale Erholung

## Trennung der Verantwortlichkeiten

Die simulierte Zeit und die geordnete Ereignisplanung gehören zum BIOS. Das
Stammhirn definiert, was ein Erholungsereignis am betroffenen Neuron verändert.
Das Netzwerk speichert ausschließlich den dazu notwendigen kurzfristigen
Neuronenzustand.

Es gibt weiterhin keinen globalen neuronalen Tick. Ein Zeitfortschritt besucht
keine Neuronen, sondern verarbeitet nur tatsächlich fällige Ereignisse.

## Simulierte Zeit

Die Laufzeit führt einen 64-Bit-Zähler `simulated_time_`. Seine Einheit ist
abstrakt. Unmittelbare Feuer- und Impulsereignisse erhalten den aktuellen
Zeitpunkt. Zukünftige Ereignisse werden nach folgendem Schlüssel geordnet:

1. geplanter Zeitpunkt,
2. monoton steigende Einfügereihenfolge.

Damit bleibt die bisherige FIFO-Reihenfolge für alle unmittelbaren Ereignisse
erhalten. Ereignisse mit gleichem zukünftigen Zeitpunkt werden deterministisch
in ihrer Planungsreihenfolge verarbeitet.

## Ereignisarten

| Art | Bedeutung |
|---|---|
| `fire` | logischer, ungewichteter Neuronenausgang |
| `impulse` | Übertragung über eine konkrete Eingangssynapse |
| `recovery` | zukünftige Erholung genau eines Neurons |
| `boundary` | technische, wirkungsfreie Versuchsgrenze |

Ein Ereignis enthält Art, Ziel-ID, Zeitpunkt, Reihenfolgenummer und bei
Erholung eine Generation. Es enthält keine Synapsenzeitstempel.

## Verarbeitungsgrenzen

`run()` verarbeitet nur Ereignisse, deren Zeitpunkt kleiner oder gleich der
aktuellen simulierten Zeit ist. Zukünftige Erholungen bleiben in der
Warteschlange. Deshalb führt weder das Ende einer Eingabe noch das Ende einer
Antwort automatisch zur Erholung.

Nur `advance_time_to()` beziehungsweise `advance_time_by()` bewegt die Zeit
ausdrücklich vorwärts. Dabei werden alle bis zur Zielzeit fälligen Ereignisse
in der definierten Reihenfolge verarbeitet. Ein Ereignislimit verhindert
unbegrenzte Kaskaden.

## Erholungsvarianten

### R0 – eingangsabhängige Referenz

R0 plant kein Erholungsereignis. Jeder weitere synaptische Eingang reduziert
den bisherigen Refraktärzähler um eins. Ohne Eingang bleibt der Zustand
unbegrenzt bestehen. Membran-, Reset- und Lernsemantik entsprechen dem Modell
vor B.1.

### R1 – eigenständige Erregbarkeitserholung

Wenn ein synaptischer Eingang ein Feuern auslöst, erhält das Neuron eine neue
Erholungsgeneration. Das BIOS plant genau ein Ereignis für
`aktuelle Zeit + recovery_delay`.

Eingänge während der Refraktärphase werden weiter verarbeitet, reduzieren den
Zähler aber nicht. Das fällige Ereignis setzt den Refraktärzustand auf 0 und
verändert weder Membranpotenzial, Synapsen noch lokalen Ereignisring. Es erzeugt
kein Feuerereignis.

### R2 – Erholung mit einmaliger Membranentspannung

R2 führt zunächst dieselbe Erregbarkeitserholung wie R1 aus. Anschließend wird
das Membranpotenzial einmalig in Richtung Ruhewert 0 skaliert:

`neues Potenzial = altes Potenzial × (100 − Relaxation in Prozent) / 100`

Untersucht werden 50 Prozent (R2H) und 100 Prozent (R2F). Dies ist kein
kontinuierliches Leck und keine synaptische Abschwächung.

## Veraltete und doppelte Ereignisse

Jedes unabhängige Feuern erhöht die Erholungsgeneration des Neurons. Ein
Erholungsereignis ist nur gültig, wenn seine Generation der aktuellen
Neuronengeneration entspricht und das Neuron noch refraktär ist. Andernfalls
wird es als veraltet gezählt und wirkungslos verworfen. Ein bereits
verarbeitetes Ereignis kann damit nicht doppelt erholen.

## Persistenz

Persistenzformat 5 speichert zusätzlich:

- Erholungsmodus, Dauer und Membranentspannung,
- aktuelle simulierte Zeit und nächste Reihenfolgenummer,
- alle ausstehenden Ereignisse samt Zeitpunkt, Reihenfolge und Generation,
- Erholungsgeneration und geplanten Zeitpunkt je Neuron,
- Erholungs- und Grenzereigniszähler.

Formate 1 bis 4 bleiben lesbar. Sie werden als R0 bei Zeit 0 ohne zukünftige
Ereignisse geladen. Ein Format-5-Zustand setzt nach Wiederherstellung dieselbe
Ereignisfolge fort.

## Komplexität und Speicher

Ein Erholungsereignis bearbeitet genau ein Neuron und löst keinen
Netzwerkdurchlauf aus. Die aktuelle geordnete Warteschlange verwendet eine
sortierte Sequenz; Verarbeitung des frühesten Ereignisses ist konstant,
Einfügen kann linear in der Zahl bereits geplanter Ereignisse sein. Das ist für
die untersuchte geringe aktive Teilmenge ausreichend, sollte bei sehr großen
gleichzeitig aktiven Populationen aber gegen eine Heap- oder Kalenderstruktur
verglichen werden.

Der erweiterte `Event` belegt auf der Messplattform 32 Byte. Generation und
geplanter Zeitpunkt erhöhen den Neuronenzustand um 16 Byte. R0 trägt diesen
Strukturplatz ebenfalls, plant aber keine Ereignisse. Für eine Million Neuronen
entsprechen die zwei neuen 64-Bit-Felder 16 MB zusätzlichem Zustand; tatsächlich
geplante Ereignisse benötigen weitere 32 Byte je Eintrag.

## Invarianten

- Erholung verändert keine Gewichte, Endpunkte oder Aktivitätszähler.
- Erholung löscht oder sortiert keinen lokalen Ereignisring.
- Erholung erzeugt kein Feuer- oder Impulsereignis.
- R0 reagiert nicht auf simulierten Zeitfortschritt.
- Ein Antwortende verarbeitet keine zukünftige Erholung.
- Gleicher Zustand, Seed und Zeitfortschritt erzeugen dieselbe Reihenfolge.
