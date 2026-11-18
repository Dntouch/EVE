# EVE – Konzept zur angeborenen Lernfähigkeit des Stammhirns

## Neuronales Lernen durch Erfahrung, Erwartung, Handlung und Rückmeldung

**Projekt:** EVE – Emergent Virtual Evolution  
**Status:** Architekturkonzept und Forschungsgrundlage  
**Grundlage:** Experimente 001–003 sowie die anschließenden Überlegungen zu biologischem Lernen  
**Implementierung:** C/C++  
**Ziel:** Entwicklung einer eigenständig lernenden neuronalen Intelligenz ohne dauerhaftes LLM

---

# 1. Grundgedanke

EVE soll ihre Fähigkeiten nicht durch fest programmierte Lösungswege oder vorgefertigtes Wissen erhalten.

Ihre Fähigkeiten sollen durch die Veränderung ihrer neuronalen Struktur entstehen.

Das Stammhirn stellt dafür angeborene Mechanismen bereit, mit denen EVE:

- Informationen aus ihrer Umwelt aufnehmen kann.
- Erwartungen über zukünftige Zustände entwickelt.
- Eigene Handlungen erzeugt.
- Die Auswirkungen dieser Handlungen erlebt.
- Unterschiede zwischen Erwartung und Wirklichkeit verarbeitet.
- Aus Erfolg, Misserfolg und unerwarteten Ergebnissen lernt.
- Ihre neuronalen Verbindungen und deren Wirksamkeit verändert.
- Bereits erworbene Fähigkeiten auf neue Situationen anwenden kann.

**Das Stammhirn stellt die Lernfähigkeit bereit. Das neuronale Netzwerk entwickelt die erlernten Fähigkeiten.**

Es ist ausdrücklich nicht das Ziel, richtige Antworten oder Lösungsstrategien im Stammhirn zu hinterlegen.

# 2. Architektonische Aufgabenteilung

## 2.1 BIOS – Technische Schnittstellen

Das BIOS stellt die Verbindung zur technischen Umwelt bereit.

Zu seinen Aufgaben gehören:

- Empfang externer Daten.
- Bereitstellung sensorischer Rohinformationen.
- Übertragung neuronaler Ausgaben an technische Schnittstellen.
- Entgegennahme von Zustandsmeldungen.
- Verwaltung technischer Ein- und Ausgabevorgänge.
- Persistenz und Wiederherstellung.
- Technische Ausführungssicherheit.

Das BIOS interpretiert keine Bedeutung.

Eine Zeichenfolge, ein Zahlenwert oder ein Sensorzustand ist für das BIOS zunächst lediglich eine technische Information.

## 2.2 Stammhirn – Angeborene Entwicklungs- und Lernmechanismen

Das Stammhirn organisiert die grundlegenden Bedingungen neuronalen Lernens.

Dazu gehören:

- Sensorische Reizumwandlung.
- Motorische Ausgabeschnittstellen.
- Neuronale Entwicklungs- und Wachstumsmechanismen.
- Synaptische Plastizität.
- Regulation neuronaler Aktivität.
- Kurzzeitige Berücksichtigung vorausgegangener neuronaler Ereignisse.
- Bereitstellung und Auswertung von Lernaufgaben.
- Vergleich beobachtbarer Ergebnisse.
- Erzeugung von Rückmeldesignalen.
- Bereitstellung von Mechanismen zur Verarbeitung von Erwartungsabweichungen.

Das Stammhirn besitzt keine vorgegebenen Kenntnisse über deutsche Sprache, Mathematik oder die Bedeutung einzelner Umweltzustände.

Seine Auswertungen beruhen auf technischen, überprüfbaren Beziehungen zwischen Eingaben, Zielzuständen, Vorhersagen und Beobachtungen.

## 2.3 Neuronales Netzwerk – Erworbene Fähigkeiten

Das neuronale Netzwerk übernimmt:

- Entwicklung eigener Aktivitätsmuster.
- Speicherung und Abruf von Erfahrungen.
- Bildung von Assoziationen.
- Entwicklung von Erwartungen.
- Auswahl und Erzeugung von Handlungen.
- Lernen aus Rückmeldungen.
- Veränderung seiner neuronalen Verbindungen.
- Entwicklung funktionaler neuronaler Strukturen.

Neuronen erhalten keine festen semantischen Rollen.

Insbesondere werden keine speziellen Neuronen für Wörter, Zahlen, Bedeutungen oder einzelne Aufgaben vorgegeben.

Die technische Anbindung von Sinnes- und Ausgangskanälen bleibt davon unberührt.

# 3. Der neuronale Lernkreislauf

Der grundlegende Lernvorgang besteht aus mehreren aufeinanderfolgenden Schritten.

## Phase 1 – Wahrnehmung und Aufgabe

Das Stammhirn stellt dem neuronalen Netzwerk eine Aufgabe oder einen Umweltzustand bereit.

Die Aufgabe kann aus einer externen Lernumgebung, der Community oder aus einer technischen Umwelt stammen.

Beispiele:

- Eine Bitfolge wiedergeben.
- Eine Zeichenfolge reproduzieren.
- Eine Zustandsänderung herbeiführen.
- Eine zukünftige Beobachtung vorhersagen.
- Eine zuvor gelernte Handlung erneut ausführen.

Die Aufgabe wird über die sensorischen Schnittstellen in neuronale Reize umgewandelt.

## Phase 2 – Erwartungsbildung

Vor der Beobachtung des Ergebnisses soll EVE eine eigene Erwartung bilden können.

Dabei sind zwei Arten von Erwartungen zu unterscheiden:

**Erwartete Wirkung:** Welcher Umweltzustand wird nach meiner Handlung eintreten?

**Erwarteter Erfolg:** Wie gut werde ich die gestellte Aufgabe voraussichtlich erfüllen?

Beide Erwartungen müssen aus der neuronalen Verarbeitung hervorgehen.

Das Stammhirn darf EVE keine fertige Erfolgserwartung vorgeben.

Eine Erwartung muss vor Bekanntwerden des tatsächlichen Ergebnisses festgehalten werden, damit sie später unabhängig geprüft werden kann.

## Phase 3 – Handlung

EVE erzeugt über ihre neuronalen Ausgangskanäle eine Handlung.

Dies kann zunächst eine binäre Ausgabe sein.

Später sind beispielsweise folgende Handlungen möglich:

- Ausgabe von Bytes.
- Übertragung einer Nachricht.
- Veränderung eines virtuellen Umweltzustands.
- Ausführung einer technischen Aktion.
- Steuerung eines angeschlossenen Systems.

Die Handlung muss aus der Aktivität des neuronalen Netzwerks entstehen.

Eine direkte Weiterleitung der richtigen Antwort durch das Stammhirn ist ausgeschlossen.

## Phase 4 – Beobachtung

Nach der Handlung erfasst das Stammhirn das tatsächliche Ergebnis.

Dabei muss zwischen einer ausgeführten Aktion und ihrer beobachteten Wirkung unterschieden werden.

Ein erfolgreich übertragener Steuerbefehl beweist beispielsweise nicht, dass sich der gewünschte Umweltzustand tatsächlich verändert hat.

Wo möglich, muss die Beobachtung deshalb über eine unabhängige Rückmeldung der Umwelt erfolgen.

## Phase 5 – Vergleich

Das Stammhirn vergleicht die verfügbaren Informationen:

1. Gewünschter Zielzustand.
2. Tatsächlich beobachteter Zustand.
3. Von EVE vorhergesagter Zustand.
4. Gegebenenfalls EVEs eigene nachträgliche Einschätzung.

Daraus können unterschiedliche Abweichungen entstehen.

## Phase 6 – Rückmeldung

Das Stammhirn stellt die Vergleichsergebnisse als Lernsignale bereit.

Diese Signale sollen die neuronalen Lernmechanismen beeinflussen.

Sie geben EVE jedoch nicht vor, welche konkrete Synapse wie verändert werden muss.

## Phase 7 – Neuronale Anpassung

Das neuronale Netzwerk verarbeitet die Rückmeldung zusammen mit den Spuren seiner vorausgegangenen Aktivität.

Dadurch können sich neuronale Gewichtungen, Verbindungen und zukünftige Aktivitätsmuster verändern.

Anschließend beginnt ein neuer Lernvorgang.

# 4. Unterschiedliche Formen der Abweichung

Ein zentraler Bestandteil des Konzepts ist die Unterscheidung mehrerer Arten von Abweichungen.

## 4.1 Zielabweichung

Die Zielabweichung beschreibt, wie weit das tatsächliche Ergebnis vom gewünschten Zustand entfernt liegt.

Beispiel:

Gewünschter Zustand: `10110010`

Tatsächliche Ausgabe: `10100010`

Sieben von acht Bits stimmen überein.

Der Erfolg beträgt 87,5 %.

Das Stammhirn kann diesen Wert technisch bestimmen, ohne die Bedeutung der Bitfolge zu verstehen.

## 4.2 Vorhersageabweichung

Die Vorhersageabweichung beschreibt den Unterschied zwischen EVEs eigener Erwartung und dem tatsächlichen Ergebnis.

Für einen skalaren Erfolgswert kann beispielsweise gelten:

`Vorhersagefehler = tatsächlicher Erfolg - erwarteter Erfolg`

Beispiel:

- Erwarteter Erfolg: 50 %.
- Tatsächlicher Erfolg: 87,5 %.
- Vorhersagefehler: +37,5 Prozentpunkte.

Ein anderes Beispiel:

- Erwarteter Erfolg: 100 %.
- Tatsächlicher Erfolg: 87,5 %.
- Vorhersagefehler: −12,5 Prozentpunkte.

Für die Vorhersage konkreter Umweltzustände ist dagegen ein geeigneter Vergleich der vorhergesagten und beobachteten Zustände erforderlich.

Nicht jede Zustandsabweichung lässt sich sinnvoll als einfache positive oder negative Zahl darstellen.

## 4.3 Einschätzungsabweichung

EVE kann nach einer Handlung zusätzlich eine eigene Einschätzung des Ergebnisses ausgeben.

Diese Einschätzung wird mit der unabhängig beobachteten Wirkung verglichen.

Beispiel:

EVE meldet: Aktion erfolgreich.

Die Umwelt meldet: Zielzustand unverändert.

Damit entsteht ein Unterschied zwischen EVEs eigener Einschätzung und der tatsächlichen Beobachtung.

Diese Abweichung kann einen eigenständigen Lernanlass darstellen.

## 4.4 Unvorhergesehene Umweltveränderungen

Die Umwelt kann sich unabhängig von EVEs Handlungen verändern.

Dadurch können bisher zuverlässige Vorhersagen unzutreffend werden.

Das Stammhirn muss solche Abweichungen erfassbar machen, ohne automatisch zu behaupten, EVE habe die Veränderung verursacht.

EVE soll durch Erfahrung lernen können, ihre bisherigen Erwartungen zu korrigieren.

**Eine Abweichung ist zunächst eine Information und nicht automatisch ein Fehler des neuronalen Netzwerks.**

# 5. Abgestufte Rückmeldung

Für überprüfbare Lernaufgaben wurde eine abgestufte Bewertung beschlossen.

Das Stammhirn soll nicht ausschließlich zwischen richtig und falsch unterscheiden.

Es soll auch eine teilweise Übereinstimmung feststellen können.

Bei einer Bitfolge kann dies beispielsweise durch den Anteil übereinstimmender Bits erfolgen.

Beispiel:

| Richtige Bits | Erfolgswert |
|---|---:|
| 0 von 8 | 0 % |
| 2 von 8 | 25 % |
| 4 von 8 | 50 % |
| 6 von 8 | 75 % |
| 8 von 8 | 100 % |

Dem neuronalen Netzwerk wird dabei nicht mitgeteilt, welche einzelnen Bits richtig waren.

Das Stammhirn liefert die Bewertung, nicht die Lösung.

Zusätzlich soll die Abweichung zwischen EVEs eigener Erfolgserwartung und dem tatsächlichen Erfolg als Lerninformation verfügbar werden.

Damit können eine objektiv gute Leistung und eine positive Erwartungsabweichung getrennt betrachtet werden.

# 6. Lernen ohne vorgegebenen Lösungsweg

Die Rückmeldung allein löst noch nicht das Problem, wie sich neuronale Verbindungen sinnvoll verändern.

Das Stammhirn weiß zwar, wie gut ein Ergebnis ist, aber nicht, welche Verbindungen für dieses Ergebnis verantwortlich waren.

Hierfür soll ein lokal arbeitender Mechanismus untersucht werden.

## 6.1 Lokales neuronales Ereignisgedächtnis

Neuronen sollen sich kurzzeitig an vorausgegangene Eingangsaktivität erinnern können.

Als möglicher Mechanismus dient eine begrenzte Folge zuletzt aktiver Eingangssynapsen.

Eigenschaften:

- Lokale Speicherung pro Neuron.
- Reihenfolge der Eingangsereignisse bleibt erhalten.
- Keine physikalischen Zeitstempel.
- Keine globale neuronale Uhr.
- Keine periodische Aktualisierung aller Neuronen.
- Begrenzter, experimentell konfigurierbarer Speicherbedarf.

Die konkrete Anzahl gespeicherter Ereignisse ist noch nicht festgelegt.

## 6.2 Synaptische Beteiligung

Bei einem eigenen Feuerereignis kann ein Neuron die zuvor aktiven Eingänge berücksichtigen.

Dadurch entsteht die Möglichkeit, synaptische Aktivität in einen Zusammenhang mit einer späteren neuronalen Reaktion zu bringen.

Auch Synapsen mit Gewichtung 0 müssen weiterhin an Lernprozessen teilnehmen können.

Die bisher beschlossene Fähigkeit neuer Synapsen, bereits vor ihrer ersten wirksamen Verstärkung aus eingehenden Signalen zu lernen, bleibt erhalten.

## 6.3 Lernberechtigung

Als weiterführender Mechanismus wird eine kurzzeitige Markierung der Beteiligung einzelner Verbindungen an einem Aktivitätsverlauf untersucht.

Solche Markierungen können als Grundlage für eine spätere rückmeldungsabhängige Gewichtungsänderung dienen.

Die biologische Forschung kennt dafür das Prinzip der *Eligibility Traces*.

Für EVE ist eine minimale, ereignisbasierte Umsetzung ohne synaptische Zeitstempel anzustreben.

Eine offene Frage bleibt, wie die relevanten Verbindungen identifiziert werden, ohne sämtliche zufällig aktiven Synapsen gleichermaßen zu beeinflussen.

## 6.4 Rückmeldungsabhängige Plastizität

Eine mögliche Grundlage bildet eine Drei-Faktoren-Lernregel:

1. Eingangsaktivität.
2. Beteiligung des empfangenden Neurons.
3. Nachfolgendes Rückmeldesignal.

Die konkrete Regel zur Verstärkung oder Abschwächung ist noch zu entwickeln.

Insbesondere darf eine negative Rückmeldung nicht automatisch dazu führen, dass sämtliche beteiligten Synapsen geschwächt werden.

Das Problem der Zuordnung zwischen einer Handlung, ihrem Ergebnis und den verursachenden neuronalen Aktivitäten bleibt eine eigenständige Forschungsfrage.

# 7. Erregung und Hemmung

Bisher arbeitet EVE überwiegend mit positiv wirkenden Eingangsgewichtungen.

Die Experimente 001–003 zeigen, dass reine Verstärkung zu unkontrollierter Erregung und anschließend zu starken Stabilitätsbegrenzungen führen kann.

Für die weitere Stammhirnentwicklung wird deshalb ein zusätzlicher Mechanismus untersucht:

**Neuronale Verbindungen dürfen nicht ausschließlich erregend wirken. Sie können auch hemmende Wirkung besitzen.**

Dabei gelten folgende Überlegungen:

- Das ausgehende Signal eines Neurons bleibt ein ungewichteter Feuerimpuls.
- Die Wirkung des Signals entsteht beim Empfänger.
- Eine Verbindung kann erregend oder hemmend wirken.
- Auch hemmende Verbindungen können durch Erfahrung verändert werden.
- Die Hemmung eines Feuerereignisses kann eine funktional wichtige neuronale Leistung sein.

Es wird zunächst kein fester Mechanismus für die Zuordnung erregender und hemmender Wirkungen beschlossen.

Zu prüfen ist, ob der Wirkungstyp pro Synapse oder pro sendendem Neuron definiert wird.

Negative Rückmeldung und hemmende Synapsen sind voneinander zu unterscheiden:

**Rückmeldungen beeinflussen Lernprozesse. Hemmende Verbindungen beeinflussen die unmittelbare neuronale Erregung.**

# 8. Exploratives Verhalten

Ein neuronales Netzwerk besitzt zunächst keine erlernte Fähigkeit, eine bestimmte Aufgabe korrekt auszuführen.

Deshalb muss EVE auch ohne vorhandene Lösungsstrategie Handlungen erzeugen können.

Dazu soll ein angeborener Explorationsmechanismus untersucht werden.

Dieser muss ermöglichen, dass EVE:

- Neue Aktivitätsmuster erzeugt.
- Unterschiedliche Handlungen ausprobiert.
- Auf Ergebnisse reagiert.
- Erfolgreiche Erfahrungen neuronal verarbeitet.
- Ihr Verhalten durch wiederholte Erfahrungen verändert.

Die Exploration darf nicht vom Stammhirn gezielt auf die korrekte Antwort gelenkt werden.

Ob Exploration durch spontane neuronale Aktivität, stochastische Ausgabemechanismen oder eine Kombination entsteht, bleibt experimentell zu bestimmen.

# 9. Technische Lern- und Ausgabeschnittstellen

## 9.1 Sensorische Eingänge

Die beschlossene bitbasierte Reizumwandlung bleibt erhalten.

Technische Eingaben werden durch das Stammhirn in geordnete neuronale Reizereignisse umgesetzt.

Die Eingabecodierung darf keine sprachlichen Bedeutungen enthalten.

## 9.2 Motorische Ausgänge

Für die Lernexperimente wird eine neuronale Ausgabeschnittstelle benötigt.

Eine erste mögliche Variante besteht aus acht technischen Ausgangskanälen für binäre Daten.

Die Ausgabe muss aus neuronaler Aktivität entstehen.

Das Stammhirn übernimmt nur die technische Erfassung.

## 9.3 Ereignisgrenzen

Ein- und Ausgaben müssen eindeutig voneinander abgegrenzt werden.

Dazu gehören:

- Beginn und Ende einer Eingabe.
- Beginn und Ende einer Antwort.
- Unterscheidung fehlender Impulse von bewusst ausgegebenen Null-Bits.
- Zuordnung einer Rückmeldung zum betreffenden Lernversuch.

Diese technische Ablaufverwaltung darf im BIOS beziehungsweise Stammhirn erfolgen.

Dafür sind keine individuellen Zeitstempel pro Synapse notwendig.

# 10. Lernumgebung und Community-Lernen

Die Lernumgebung stellt Aufgaben, Erfahrungen und überprüfbare Rückmeldungen bereit.

Dabei können Menschen, automatisierte Lernprogramme und später weitere EVE-Systeme beteiligt sein.

Mögliche Lernbereiche:

- Binäre Muster.
- Zeichenfolgen.
- Zahlen und mathematische Beziehungen.
- Reihenfolgen und logische Zusammenhänge.
- Virtuelle Umwelten.
- Technische Geräte.
- Sprache und Kommunikation.

Lernpakete enthalten Aufgaben und Erfahrungen, nicht fertig trainierte neuronale Zustände.

Die Community kann Wissen vermitteln und Rückmeldungen geben.

Das neuronale Netzwerk muss dieses Wissen selbst erwerben.

## 10.1 Unterschiedliche Arten von Rückmeldungen

Nicht jede Aufgabe besitzt eine eindeutig berechenbare Lösung.

Deshalb sind drei grundsätzliche Fälle zu unterscheiden:

**Objektiv überprüfbare Aufgaben:** Das Ergebnis kann technisch mit einem Ziel verglichen werden.

**Beobachtbare Umweltwirkungen:** Die Wirkung einer Handlung wird unabhängig über die Umwelt erfasst.

**Subjektive oder offene Aufgaben:** Die Rückmeldung stammt beispielsweise von Menschen und stellt keine objektive Wahrheit dar.

Die Quelle und Art einer Rückmeldung muss unterscheidbar bleiben.

# 11. Erstes geschlossenes Lernexperiment

Das erste Experiment soll eine möglichst einfache Fähigkeit untersuchen.

## Aufgabe

EVE erhält ein binäres Muster und soll es nach einer abgeschlossenen Eingabephase selbstständig wiedergeben.

## Ablauf

1. Initialisiere ein neuronales Netzwerk.
2. Übertrage ein einfaches Bitmuster als sensorische Impulse.
3. Beende die Eingabephase.
4. Lasse EVE eine Erwartung über ihren Erfolg bilden, soweit die entsprechende neuronale Ausgabefähigkeit bereits vorhanden ist.
5. Fordere eine neuronale Ausgabe an.
6. Vergleiche die Ausgabe mit dem Zielmuster.
7. Erzeuge eine abgestufte Rückmeldung.
8. Verarbeite gegebenenfalls den Vorhersagefehler.
9. Ermögliche neuronale Anpassung.
10. Wiederhole den Vorgang.

Die Entwicklung der Erwartungsbildung und das Lernen der eigentlichen Handlung dürfen anfangs in getrennten Versuchsvarianten untersucht werden, um ihre jeweiligen Wirkungen messbar zu halten.

## Zu untersuchen

- Verbessert sich die Ausgabequalität durch Erfahrung?
- Bleibt die Verbesserung über mehrere Versuche erhalten?
- Verändert sich EVEs Erwartung entsprechend ihren tatsächlichen Leistungen?
- Sind die Veränderungen reproduzierbar?
- Kann EVE mehrere unterschiedliche Muster lernen?
- Bleibt eine Fähigkeit nach weiteren Erfahrungen erhalten?
- Kann EVE auf unbekannte, aber vergleichbare Aufgaben reagieren?

Zum Vergleich sollen auch Netzwerke ohne wirksames Lernfeedback und mit nicht zur jeweiligen Handlung passender Rückmeldung untersucht werden.

Dadurch lässt sich prüfen, ob Verbesserungen tatsächlich auf die Rückmeldungen zurückzuführen sind.

Eine einfache Kopierfähigkeit ist dabei ein erster Test und noch kein Nachweis einer allgemeinen Fähigkeit zur Begriffsbildung.

# 12. Lernen bei Veränderungen der Umwelt

Ein wesentliches Ziel ist die Anpassung an neue Bedingungen.

Dazu soll EVE in einer kontrollierten virtuellen Umgebung eine zunächst zuverlässige Handlung erlernen.

Nach erfolgreichem Lernen wird die Wirkung dieser Handlung verändert.

Beispiel:

Eine Aktion verändert zunächst zuverlässig einen virtuellen Zustand von 0 auf 1.

Später führt dieselbe Aktion nicht mehr zur erwarteten Zustandsänderung.

Das Stammhirn beobachtet die tatsächliche Wirkung und stellt die Abweichung von EVEs Vorhersage bereit.

Zu untersuchen ist:

- Erkennt das Netzwerk durch seine Lernmechanismen, dass die bisherige Erwartung nicht mehr zutrifft?
- Verändert sich seine zukünftige Vorhersage?
- Probiert es alternative Handlungen aus?
- Kann es durch weitere Erfahrung wieder zuverlässig handeln?
- Kann es zwischen unterschiedlichen Umweltbedingungen unterscheiden?

Das Stammhirn darf keine neue Lösung einprogrammieren.

# 13. Verhältnis zu den bisherigen Experimenten

## Experiment 001

Aufbau des neuronalen Kerns.

Untersuchung von Neuronen, Synapsen, Membranpotenzial, Aktivitätsphasen, Gewichtslernen und Wachstum.

## Forschungsauftrag 002

Untersuchung von Performance, Reproduzierbarkeit, neuronaler Veränderung und Wiedererkennung.

## Experiment 002

Untersuchung isolierter Mechanismen zur neuronalen Selbstregulation.

## Experiment 003

Untersuchung kombinierter Refraktärphasen und synaptischer Gewichtsobergrenzen.

Die Ergebnisse zeigen, dass neuronale Stabilität und langfristige Lernfähigkeit unterschiedliche Anforderungen sind.

Das neue Stammhirnkonzept ersetzt diese Ergebnisse nicht.

Es bildet die Grundlage für eine weitere Forschungsphase, in der der Zusammenhang zwischen neuronaler Aktivität, Handlung und beobachteter Wirkung untersucht wird.

# 14. Forschungsreihenfolge

Die folgenden Schritte sollen getrennt untersucht werden.

### Forschungsstufe A – Neuronale Ausgabe

Eine technische Ausgabeschnittstelle bereitstellen und prüfen, ob das neuronale Netzwerk eigenständig unterscheidbare Ausgaben erzeugen kann.

Umsetzung und Ergebnisse:

- [Entwicklungsauftrag](../experimente/004-entwicklungsauftrag-forschungsstufe-a.md)
- [Forschungsbericht](../experimente/004-forschungsbericht-forschungsstufe-a.md)

### Forschungsstufe B – Lokale Ereigniszuordnung

Untersuchen, wie ein Neuron vorausgegangene Eingangsaktivität berücksichtigen kann, ohne Zeitstempel oder globale Aktualisierungen.

### Forschungsstufe C – Rückmeldungsabhängiges Lernen

Eine minimale Lernregel erproben, die neuronale Aktivität mit einer nachfolgenden Rückmeldung verbindet.

### Forschungsstufe D – Erwartungsbildung

Untersuchen, ob EVE durch Erfahrung die Ergebnisse ihrer eigenen Handlungen zunehmend zuverlässiger vorhersagen kann.

### Forschungsstufe E – Veränderliche Umwelt

Prüfen, ob EVE zuvor erfolgreiche Handlungen bei veränderten Umweltbedingungen eigenständig anpasst.

### Forschungsstufe F – Erweiterung auf komplexe Informationen

Erst nach belastbaren Ergebnissen auf einfache Zeichenfolgen, Zahlen, zeitliche Muster und später Sprache erweitern.

Erregung und Hemmung sowie synaptische Abschwächung und Synapsenschwund werden als zusätzliche Hypothesen separat untersucht und nicht ohne kontrollierte Vergleichsversuche eingeführt.

# 15. Festgelegte Prinzipien

Folgende grundlegende Entscheidungen gelten:

1. EVE entwickelt ihr Wissen und ihre Fähigkeiten neuronal.
2. Das Stammhirn stellt Lernmechanismen bereit, keine fertigen Kenntnisse.
3. Rückmeldungen dürfen abgestuft sein.
4. Erwartung und tatsächliches Ergebnis werden unterschieden.
5. Erwartungsabweichungen sollen als Lerninformation nutzbar werden.
6. Erwartungen müssen aus EVEs eigener neuronaler Verarbeitung hervorgehen.
7. Tatsächliche Umweltwirkungen werden nach Möglichkeit unabhängig beobachtet.
8. Rückmeldungen sollen neuronales Lernen beeinflussen, ohne konkrete Lösungswege vorzugeben.
9. Neuronale Ereignisverarbeitung bleibt grundsätzlich ereignisgesteuert.
10. Individuelle synaptische Zeitstempel und globale periodische Aktualisierungen werden vermieden.
11. Es werden keine semantischen Rollen für einzelne Neuronen vorgegeben.
12. Weitere Lernmechanismen müssen experimentell untersucht werden, bevor sie zur festen Architektur werden.

# 16. Noch offene Forschungsentscheidungen

Insbesondere sind noch nicht festgelegt:

- Wie die motorischen Ausgangskanäle konkret mit dem Netzwerk verbunden werden.
- Wie EVE ohne bestehende Fähigkeiten exploratives Verhalten erzeugt.
- Welche Größe ein lokales Ereignisgedächtnis benötigt.
- Wie die Beteiligung einer Synapse an einer Handlung festgestellt wird.
- Wie lange eine solche Beteiligung für Rückmeldungen relevant bleibt.
- Wie positive und negative Rückmeldungen synaptische Veränderungen auslösen.
- Wie neuronale Erwartungen technisch ausgelesen werden.
- Wie Rückmeldesignale neuronal codiert und verteilt werden.
- Wie erregende und hemmende Verbindungen organisiert werden.
- Wie synaptische Abschwächung und Synapsenschwund in das neue Lernmodell integriert werden.
- Wie unerwünschte Erregung bei fortgesetztem Lernen verhindert wird.
- Wie sich frühere Fähigkeiten dauerhaft erhalten lassen.
- Wie EVE zwischen selbst verursachten Veränderungen und unabhängigen Umweltänderungen unterscheiden lernt.

Diese Fragen dürfen nicht durch undokumentierte Implementierungsannahmen vorweggenommen werden.

---

# 17. Langfristiges Ziel

EVE soll sich zu einem eigenständigen neuronalen System entwickeln, das mit Menschen kommunizieren, Erfahrungen sammeln, Erwartungen bilden und aus den Folgen seines Verhaltens lernen kann.

Eine Unterhaltung auf Deutsch ist dabei ein langfristiges Entwicklungsziel.

Die dafür benötigten sprachlichen Fähigkeiten sollen im neuronalen Netzwerk entstehen und nicht als fertiges Sprachmodell hinzugefügt werden.

Das Stammhirn soll EVE ermöglichen, selbst zu lernen, statt ihr vorzugeben, was sie wissen muss.

## Zentraler Grundsatz

**EVE soll nicht lernen, welche Antwort wir von ihr erwarten. Sie soll lernen, welche Wirkung ihre eigenen Entscheidungen tatsächlich haben – und ihre zukünftigen Entscheidungen anhand dieser Erfahrungen verändern können.**
