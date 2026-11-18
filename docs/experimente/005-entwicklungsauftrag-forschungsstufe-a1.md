# EVE – Forschungsstufe A.1
## Angeborene neuronale Organisation, Exploration und sensomotorische Rückkopplung

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-a-neural-output`  
**Status:** Entwicklungs- und Forschungsauftrag  
**Implementierung:** C++17

---

# 1. Ausgangssituation

Forschungsstufe A hat die technische Grundlage für neuronale Ausgaben geschaffen.

Vorhanden sind:

- Acht technische motorische Datenkanäle.
- Ein separater neuronaler Abschlusskontakt.
- Ein unabhängiger sensorischer Antwortreiz.
- Reproduzierbare Zuordnungen von Neuronen zu technischen Ausgängen.
- Ereignisgesteuerte Verarbeitung.
- Eine klare Unterscheidung zwischen gültiger Antwort, fehlender Antwort und abgebrochenem Versuch.
- Automatisierte Tests und reproduzierbare Forschungsabläufe.

Die Untersuchung umfasste 249 Versuchsläufe.

Ergebnisse:

- 153 isolierte Läufe ohne Lernen und Wachstum erzeugten keine motorische Ausgangsaktivität.
- In 10 von 96 Läufen mit den vorhandenen Lern- und Wachstumsregeln wurden Datenkanäle aktiviert.
- Kein Versuch erzeugte einen neuronalen Antwortabschluss.
- Die technische Ausgabeschnittstelle funktioniert.
- Das neuronale Netzwerk besitzt noch keinen zuverlässigen Mechanismus zur eigenständigen Erzeugung und Beendigung von Handlungen.

Ein wesentlicher Grund liegt möglicherweise in der bisherigen Netzwerkorganisation.

Die anfänglichen Synapsen besitzen Gewichtung 0. Gleichzeitig werden technische Ausgänge bisher überwiegend zufällig vorhandenen Neuronen zugeordnet.

Daraus folgt nicht automatisch eine funktionierende Verbindung zwischen sensorischen Reizen, neuronaler Verarbeitung und motorischer Aktivität.

**Forschungsstufe A.1 untersucht deshalb, welche angeborene neuronale Grundorganisation erforderlich ist, damit EVE eigene Handlungen hervorbringen und deren Auswirkungen erfahren kann.**

---

# 2. Übergeordnetes Forschungsziel

Untersuche vier biologisch inspirierte Mechanismen:

1. Angeborene funktionale Grundverschaltung.
2. Spontane beziehungsweise explorative neuronale Aktivität.
3. Angeborene motorische Grundschaltungen und Antwortabschluss.
4. Sensorische Rückmeldung über eigene motorische Aktivität.

Die Mechanismen werden aufeinander aufbauend implementiert und untersucht.

Dabei sollen sie möglichst unabhängig voneinander konfigurierbar bleiben.

Nach Abschluss der Einzeluntersuchungen wird ihr gemeinsames Verhalten überprüft.

## Zentrale Forschungsfrage

**Welche minimale angeborene Organisation benötigt EVE, um ohne vorgegebenes Wissen eigenständige neuronale Handlungen zu erzeugen, abzuschließen und deren Auswirkungen anschließend als neue sensorische Erfahrung zu verarbeiten?**

---

# 3. Verbindliche Architekturprinzipien

Die bestehende Trennung zwischen BIOS, Stammhirn und neuronalem Netzwerk bleibt erhalten.

## 3.1 BIOS

Das BIOS verwaltet die technischen Rahmenbedingungen:

- Rohdateneingänge und technische Ausgänge.
- Ereigniswarteschlangen.
- Versuchskonfiguration.
- Persistenz.
- Diagnose und Messung.
- Verbindung zu einer späteren Umwelt.

Das BIOS erzeugt keine inhaltlichen Antworten.

## 3.2 Stammhirn

Das Stammhirn stellt angeborene Entwicklungsmechanismen bereit.

Für diese Forschungsstufe darf es insbesondere:

- Eine initiale funktionale Netzwerkorganisation erzeugen.
- Explorative Aktivierung auslösen.
- Motorische Grundschaltungen bereitstellen.
- Sensorische Rückmeldungen aus technischen Ausgangsereignissen erzeugen.

Das Stammhirn darf keine Antworten aus Eingaben berechnen.

## 3.3 Neuronales Netzwerk

Das neuronale Netzwerk verarbeitet sämtliche Reize durch neuronale Aktivität.

Ausgaben müssen durch tatsächliche Feuerereignisse entstehen.

Es werden keine neuronalen Bedeutungen wie bestimmte Wörter, Zahlen oder Antworten vorgegeben.

## 3.4 Technische Grenzen

Beizubehalten sind:

- C++17.
- Ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Keine Zeitstempel pro Synapse.
- Keine kontinuierlichen vollständigen Netzwerkdurchläufe.
- Reproduzierbare Zufallsinitialisierung.
- Kompakte Speicherstrukturen.
- Bestehende Persistenzmechanismen.
- Keine externen KI-Modelle.

Zusätzliche Zustände sind nur einzuführen, wenn sie für einen konkret untersuchten Mechanismus erforderlich sind.

---

# 4. Forschungsphase I – Angeborene funktionale Grundverschaltung

## 4.1 Ziel

Untersuche, ob eine angeborene Netzwerkorganisation die zuverlässige Übertragung von sensorischer Aktivität bis zu motorischen Kontakten ermöglicht.

Hierfür soll das Netzwerk nicht ausschließlich durch zufällige Verbindungen initialisiert werden.

Es erhält zusätzlich eine begrenzte Menge angeborener Verbindungen, welche die grundsätzliche Kommunikation zwischen sensorischen, verarbeitenden und motorischen Bereichen ermöglichen.

Diese Organisation darf keine richtige Ausgabe vorgeben.

## 4.2 Grundgedanke

In biologischen Nervensystemen entstehen bereits während der Entwicklung grundlegende anatomische Verbindungen.

Ein Neugeborenes muss nicht erst durch Zufall eine vollständige Verbindung zwischen seinen Sinnesorganen, seinem Nervensystem und seinen Muskeln herstellen.

Es besitzt grundlegende Verschaltungen, deren Koordination und Wirksamkeit sich durch Erfahrung verändern.

Für EVE soll eine vergleichbare funktionale Ausgangsorganisation untersucht werden.

## 4.3 Technische Umsetzung

Ergänze eine optionale initiale Netzwerkorganisation.

Diese berücksichtigt:

- Sensorische Eingangskontakte.
- Den vorhandenen Antwortreiz.
- Allgemeine neuronale Verarbeitungsbereiche.
- Motorische Datenkontakte.
- Den motorischen Abschlusskontakt.

Die Positionen ergeben sich weiterhin aus dem einmalig gefalteten dreidimensionalen neuronalen Raum.

Die Grundverschaltung kann zusätzliche gerichtete Synapsen anlegen.

Dabei gelten weiterhin:

- Keine doppelten gerichteten Synapsen.
- Keine Selbstverbindungen.
- Normale synaptische Eingangsgewichtungen.
- Normale neuronale Feuerereignisse.
- Keine Umgehung des neuronalen Netzwerks.

## 4.4 Experimentelle Varianten

Untersuche mindestens:

**V0 – Bisherige Referenz**

Vollständig zufällige initiale Vernetzung mit den bisherigen Startgewichten.

**V1 – Strukturelle Erreichbarkeit**

Die initiale Organisation stellt gerichtete Verbindungspfade zwischen sensorischen, verarbeitenden und motorischen Bereichen sicher.

Neue Verbindungen erhalten Gewichtung 0.

**V2 – Funktionale Erreichbarkeit**

Wie V1, jedoch besitzen ausgewählte angeborene Verbindungen eine geringe positive Anfangsgewichtung.

Die dafür notwendigen Werte sind konfigurierbare Versuchsparameter.

**V3 – Funktionale Erreichbarkeit mit variierter Topologie**

Wie V2, jedoch mit reproduzierbar unterschiedlichen initialen Anordnungen und Verbindungspfaden.

## 4.5 Wichtige Abgrenzung

Die Verschaltung darf keine feste Zuordnung wie Eingangsbit 3 zu Ausgangsbit 3 enthalten.

Sie muss lediglich ermöglichen, dass sensorische Aktivität motorische Bereiche erreichen kann.

Es ist zwischen zwei Eigenschaften zu unterscheiden:

**Strukturelle Erreichbarkeit:** Ein gerichteter synaptischer Verbindungspfad existiert.

**Funktionale Erreichbarkeit:** Ein Reiz kann unter definierten Bedingungen tatsächlich eine Aktivierung entlang dieses Pfades auslösen.

Die bloße Existenz eines Pfades ist kein Nachweis für funktionale Signalübertragung.

## 4.6 Untersuchungen

Ermittle:

- Wie viele motorische Kontakte sind strukturell erreichbar?
- Wie viele sind durch neuronale Aktivität tatsächlich aktivierbar?
- Wie viele Neuronen liegen zwischen Eingabe und motorischem Kontakt?
- Welche Rolle spielen die Anfangsgewichtungen?
- Wie viele zusätzliche Synapsen sind für funktionale Erreichbarkeit erforderlich?
- Unterscheidet sich das Ergebnis bei verschiedenen initialen Topologien?
- Entstehen dadurch unkontrollierte Feuerkaskaden?

## 4.7 Abschluss der Forschungsphase

Die Phase soll nachweisen oder widerlegen, ob angeborene Grundverschaltungen gegenüber der bisherigen Zufallsvernetzung die funktionale Erreichbarkeit motorischer Kontakte verbessern.

Die Ergebnisse werden dokumentiert.

Die erfolgversprechenden Topologien werden anschließend als Varianten für Forschungsphase II übernommen.

Eine endgültige Architekturentscheidung erfolgt noch nicht.

---

# 5. Forschungsphase II – Explorative neuronale Aktivität

## 5.1 Ziel

Untersuche, wie EVE auch ohne bereits erlernte Handlungsstrategie eigene neuronale Aktivität entwickeln kann.

Bisher benötigt das ereignisgesteuerte Netzwerk einen auslösenden Reiz.

Ohne vorhandene Aktivität gibt es keine Feuerereignisse, kein Wachstum und keine motorischen Handlungen.

Das entspricht nicht vollständig dem Entwicklungsverhalten biologischer Nervensysteme, in denen spontane neuronale Aktivitätsmuster vorkommen.

## 5.2 Grundprinzip

Das Stammhirn soll eine begrenzte explorative Aktivität ermöglichen.

Diese darf:

- Neuronen aktivieren.
- Neuronale Aktivitätsmuster anstoßen.
- Unterschiedliche neuronale Verarbeitungsverläufe ermöglichen.

Sie darf nicht:

- Ein fertiges Byte generieren.
- Eine richtige Antwort auswählen.
- Die vorhandene Eingabe unmittelbar ausgeben.
- Den motorischen Ausgang außerhalb des neuronalen Netzwerks manipulieren.

**Exploration bedeutet neuronales Ausprobieren, nicht zufällige Antworterzeugung durch das Stammhirn.**

## 5.3 Experimentelle Varianten

Untersuche mindestens:

**E0 – Ohne Exploration**

Referenz mit unveränderter Aktivitätsauslösung.

**E1 – Einzelne explorative Aktivierung**

Das Stammhirn erzeugt ein kontrolliertes Feuerereignis eines reproduzierbar zufällig ausgewählten Neurons.

**E2 – Mehrere explorative Aktivierungen**

Innerhalb eines Versuchs werden mehrere ausgewählte Neuronen nacheinander aktiviert.

**E3 – Räumlich begrenzte Exploration**

Explorative Aktivierungen werden auf kleine Bereiche des virtuellen dreidimensionalen Netzwerks beschränkt.

**E4 – Verteilte Exploration**

Explorative Reize werden über unterschiedliche Bereiche des Netzwerks verteilt.

## 5.4 Ereignissteuerung

Exploration muss vollständig mit der vorhandenen ereignisgesteuerten Architektur vereinbar sein.

Es sollen keine zusätzlichen individuellen Zeitstempel eingeführt werden.

Für die erste Umsetzung dürfen Explorationsereignisse an definierte Versuchsschritte gebunden sein.

Dies kann beispielsweise die Einleitung einer Antwortphase sein.

Ein dauerhaft autonomer Explorationsbetrieb ohne äußere Aufgaben bleibt eine spätere Erweiterung.

## 5.5 Reproduzierbarkeit

Die Auswahl explorativ aktivierter Neuronen muss bei gleichem Seed reproduzierbar sein.

Der für Exploration verwendete Zufallszustand soll getrennt vom Zufallszustand der Netzwerkentwicklung verwaltet werden, damit unterschiedliche Explorationsvarianten nicht unbeabsichtigt die initiale Vernetzung verändern.

Alle erzeugten explorativen Ereignisse sind zu protokollieren.

## 5.6 Messgrößen

Untersuche:

- Anzahl ausgelöster explorativer Ereignisse.
- Daraus entstehende neuronale Feuerereignisse.
- Aktivierte motorische Kontakte.
- Unterschiedliche erzeugte Bitmuster.
- Räumliche Ausbreitung der Aktivität.
- Veränderungen durch wiederholte Exploration.
- Maximale Ereigniswarteschlange.
- Speicherverbrauch.
- Rechenaufwand.

Besondere Aufmerksamkeit gilt der Frage, ob einzelne explorative Ereignisse zu eigenständigen Aktivitätsverläufen führen oder lediglich isolierte Feuerereignisse erzeugen.

## 5.7 Sicherheits- und Stabilitätsgrenzen

Die Exploration erhält ein konfigurierbares Ereignisbudget.

Unkontrollierte Kaskaden müssen erkannt und abgebrochen werden.

Ein Abbruch ist im Forschungsbericht als Instabilität zu kennzeichnen und darf nicht als neuronaler Antwortabschluss gewertet werden.

## 5.8 Abschluss der Forschungsphase

Die Phase soll zeigen, ob EVE durch begrenzte explorative Reize unterschiedliche motorische Aktivitäten hervorbringen kann.

Die Untersuchung muss auch erfassen, wie stark das Ergebnis von der angeborenen Grundverschaltung aus Forschungsphase I abhängt.

---

# 6. Forschungsphase III – Angeborene motorische Grundschaltungen

## 6.1 Ziel

Untersuche, wie EVE eine begonnene motorische Handlung eigenständig und neuronengesteuert abschließen kann.

Bisher besitzt EVE einen eigenen Abschlusskontakt.

In Forschungsstufe A wurde dieser jedoch in keinem Versuch aktiviert.

Eine mögliche Ursache ist, dass der Abschlusskontakt lediglich zufällig vorhandenen Neuronen zugeordnet wurde und keine geeignete funktionale Verbindung zu den motorischen Aktivitätsverläufen besitzt.

## 6.2 Biologisches Vorbild

Biologische Nervensysteme besitzen angeborene motorische Grundschaltungen.

Diese können elementare Bewegungsabläufe und Reflexe erzeugen, ohne dass ihre genaue Steuerung zuvor erlernt werden muss.

Für EVE soll ein vergleichbarer Mechanismus untersucht werden.

Die Schaltung darf eine motorische Handlung organisieren.

Sie darf jedoch nicht den Inhalt der Handlung vorgeben.

## 6.3 Grundmodell

Eine motorische Grundschaltung kann aus einer begrenzten Gruppe angeboren verbundener Neuronen bestehen.

Sie kann:

- Einen motorischen Aktivitätsverlauf ermöglichen.
- Mehrere Ausgangsereignisse koordinieren.
- Die Erreichbarkeit des Abschlusskontakts unterstützen.

Das Abschlusssignal muss weiterhin durch ein tatsächliches neuronales Feuerereignis entstehen.

Eine technische Beendigung durch das BIOS ist kein neuronaler Abschluss.

## 6.4 Experimentelle Varianten

Untersuche mindestens:

**M0 – Bisheriger Abschlusskontakt**

Keine zusätzliche Grundschaltung.

**M1 – Angeborener Abschlussweg**

Der Abschlusskontakt besitzt einen funktionalen neuronalen Verbindungspfad aus dem motorischen Bereich.

**M2 – Motorische Abschlussgruppe**

Mehrere Neuronen können über eine kleine gemeinsame Schaltung ein neuronales Abschlussereignis auslösen.

**M3 – Aktivitätsabhängige Grundschaltung**

Die Schaltung kann einen Abschluss abhängig von vorausgegangener motorischer Aktivität auslösen.

Diese Variante darf keine komplexe neue Lernregel voraussetzen.

## 6.5 Unterscheidung verschiedener Abschlüsse

Für die Forschung ist unbedingt zu unterscheiden:

- Eigenständiger Abschluss aus allgemeiner Netzwerkaktivität.
- Durch eine angeborene motorische Grundschaltung unterstützter Abschluss.
- Zufälliger Abschluss durch Exploration.
- Technischer Abbruch durch das BIOS.

Diese unterschiedlichen Ursachen müssen messbar bleiben.

Ein Reflexabschluss darf nicht als Nachweis einer erlernten Entscheidung über das Ende einer Antwort interpretiert werden.

## 6.6 Gültige Nullbyte-Ausgaben

Die bisherige Unterscheidung bleibt erhalten:

Ein gültiges Nullbyte benötigt einen tatsächlich erzeugten neuronalen Abschluss.

Das bloße Ausbleiben von Ausgangsaktivität ist weiterhin keine gültige Antwort.

## 6.7 Untersuchungen

Erfasse:

- Anzahl motorischer Aktivierungen.
- Anzahl neuronaler Abschlüsse.
- Anteil abgeschlossener Antworten.
- Anzahl abgeschlossener Nullbyte-Antworten.
- Unterschiedliche ausgegebene Bitmuster.
- Aktivitätsverlauf vor einem Abschluss.
- Beteiligte Neuronen.
- Ereignisaufwand bis zum Abschluss.
- Häufigkeit zufälliger oder unmittelbarer Abschlüsse.
- Stabilität der Grundschaltung.

Untersuche insbesondere, ob die Grundschaltung zu früh abschließt, bevor sich eine differenzierte motorische Aktivität entwickeln konnte.

## 6.8 Abschluss der Forschungsphase

Forschungsphase III soll klären, welche einfachen angeborenen neuronalen Schaltungen eine zuverlässig abschließbare motorische Aktivität ermöglichen.

Eine hohe Abschlussrate allein gilt nicht als ausreichend.

Die motorischen Ausgaben müssen weiterhin unterschiedliche, durch neuronale Aktivität entstandene Muster enthalten können.

---

# 7. Forschungsphase IV – Sensorische Rückmeldung eigener Handlungen

## 7.1 Ziel

Untersuche, ob EVE die Wirkung ihrer eigenen motorischen Aktivität als neue sensorische Erfahrung wahrnehmen kann.

Dies ist ein zentraler Schritt zur späteren Entwicklung von Erwartungen und handlungsabhängigem Lernen.

Bisher erhält EVE sensorische Eingaben und kann motorische Signale ausgeben.

Zwischen diesen Vorgängen existiert jedoch noch kein geschlossener sensorischer Rückkopplungskreis.

## 7.2 Biologischer Hintergrund

Biologische Organismen erhalten über ihre Sinnesorgane Rückmeldungen über eigene Bewegungen und deren Wirkungen.

Solche Rückmeldungen tragen dazu bei, Zusammenhänge zwischen neuronaler Aktivität und den daraus folgenden Veränderungen zu lernen.

Für EVE soll zunächst eine technische Entsprechung untersucht werden.

## 7.3 Grundmodell

Wenn EVE einen motorischen Ausgang aktiviert, kann daraus ein neues sensorisches Ereignis entstehen.

Das Stammhirn stellt diese Rückmeldung über einen gesonderten sensorischen Kanal bereit.

Die Rückmeldung repräsentiert eine tatsächlich aufgetretene motorische Aktivierung.

Sie enthält keine Bewertung, ob die Handlung richtig oder falsch war.

## 7.4 Unterscheidung zwischen Eigenaktivität und Umweltwirkung

Für die spätere Entwicklung sind zwei Arten sensorischer Rückmeldung zu unterscheiden.

**Motorische Eigenrückmeldung**

Information darüber, welche motorischen Signale EVE tatsächlich erzeugt hat.

**Umweltrückmeldung**

Information darüber, welche Zustandsveränderung in der Umwelt tatsächlich beobachtet wurde.

Diese beiden Rückmeldungen dürfen nicht gleichgesetzt werden.

Ein ausgegebener Befehl bedeutet nicht automatisch, dass seine gewünschte Wirkung eingetreten ist.

Für Forschungsphase IV wird zunächst die motorische Eigenrückmeldung implementiert.

Eine einfache virtuelle Umgebung kann ergänzend genutzt werden, um beide Arten der Beobachtung getrennt zu untersuchen.

## 7.5 Experimentelle Varianten

**R0 – Keine Rückkopplung**

Die motorische Ausgabe hat keine zusätzliche sensorische Wirkung.

**R1 – Rückmeldung nach Abschluss**

Nach einer abgeschlossenen motorischen Antwort wird deren tatsächlich beobachtete Ausgangsaktivität über die sensorische Schnittstelle zurückgeführt.

**R2 – Rückmeldung einzelner motorischer Ereignisse**

Jede motorische Aktivierung kann ein entsprechendes sensorisches Ereignis erzeugen.

**R3 – Rückmeldung aus einer virtuellen Umwelt**

Die motorische Aktivität wirkt auf eine minimale virtuelle Umgebung.

Die Umgebung liefert anschließend einen unabhängig beobachteten Zustand zurück.

Für R3 ist eine einfache, deterministische Umgebung ausreichend. Eine inhaltliche Problemlösungsaufgabe wird noch nicht eingeführt.

## 7.6 Technische Vorgaben

Die Rückmeldung muss:

- Aus einer tatsächlich beobachteten Aktivität stammen.
- Reproduzierbar sein.
- Über die reguläre sensorische Verarbeitung laufen.
- Ohne vorgegebene Bedeutung verarbeitet werden.
- Von ursprünglich externen Eingabereizen unterscheidbar sein.
- Im Ereignisprotokoll nachvollziehbar bleiben.

Die Rückkopplung darf nicht direkt neuronale Gewichte verändern.

Sie erzeugt zunächst lediglich neue Wahrnehmungsereignisse.

## 7.7 Vermeidung unkontrollierter Rückkopplung

Motorische Eigenrückmeldungen können selbst neue neuronale Feuerereignisse auslösen.

Diese könnten wiederum motorische Ausgaben erzeugen.

Dadurch können geschlossene Aktivitätsschleifen entstehen.

Untersuche deshalb ausdrücklich:

- Selbstverstärkende Rückkopplungen.
- Ereignisexplosionen.
- Dauerhaft wiederkehrende Aktivitätsmuster.
- Stabilität der Ereigniswarteschlange.
- Auswirkungen auf Refraktärphasen.
- Unterschiede zwischen verzögerter und unmittelbarer ereignisbasierter Rückmeldung.

Alle Versuchsläufe benötigen ein begrenztes Ereignisbudget.

Eine Rückkopplungsschleife darf nicht durch das heimliche Unterdrücken neuronaler Ereignisse stabilisiert werden.

Falls eine experimentelle Begrenzung erforderlich ist, muss sie offen konfigurierbar und separat vermessen werden.

## 7.8 Untersuchungen

Ermittle:

- Werden motorische Aktivitäten zuverlässig sensorisch zurückgemeldet?
- Entstehen dadurch neue neuronale Aktivitätsmuster?
- Unterscheiden sich Netzwerke mit und ohne Eigenrückmeldung?
- Führt die Rückkopplung zu zusätzlicher neuronaler Aktivität?
- Bleibt der Zusammenhang zwischen Ausgang und Rückmeldung reproduzierbar?
- Kann das Netzwerk unterschiedliche eigene Ausgaben als unterschiedliche sensorische Erfahrungen verarbeiten?
- Welche zusätzlichen Speicher- und Rechenkosten entstehen?

## 7.9 Abschluss der Forschungsphase

Forschungsphase IV ist abgeschlossen, wenn nachgewiesen werden kann, dass EVE ihre eigene motorische Aktivität über die sensorische Schnittstelle als neues neuronales Ereignis erfahren kann.

Die bloße technische Rückmeldung gilt noch nicht als erlerntes Selbstverständnis oder als Nachweis von Erwartungsbildung.

---

# 8. Gemeinsamer Integrationsversuch

Nach Abschluss aller vier Forschungsphasen werden die untersuchten Mechanismen kombiniert.

Ziel ist ein vollständiger sensomotorischer Ablauf.

## Ablauf

1. Das BIOS empfängt eine Eingabe.
2. Das Stammhirn wandelt sie in sensorische Impulse um.
3. Die angeborene Grundverschaltung ermöglicht ihre neuronale Weiterleitung.
4. Das Stammhirn stellt einen Antwortreiz bereit.
5. Explorative Aktivität kann unterschiedliche neuronale Verarbeitungsverläufe anstoßen.
6. Neuronale Aktivität erreicht motorische Ausgangskontakte.
7. Eine motorische Grundschaltung ermöglicht einen neuronalen Abschluss.
8. Die tatsächliche motorische Aktivität wird erfasst.
9. Das Stammhirn erzeugt eine sensorische Eigenrückmeldung.
10. Das neuronale Netzwerk verarbeitet diese Rückmeldung als neue Erfahrung.

Es findet dabei noch keine Bewertung der Richtigkeit statt.

Auch eine gezielte rückmeldungsabhängige synaptische Verstärkung wird noch nicht eingeführt.

## Forschungsfragen

- Entstehen unterschiedliche neuronale Ausgaben?
- Können die Ausgaben zuverlässig abgeschlossen werden?
- Verändert sich die neuronale Aktivität durch sensorische Eigenrückmeldung?
- Bleibt das System stabil?
- Welche Grundorganisation ist erforderlich?
- Welche der vier Mechanismen tragen tatsächlich zum Ergebnis bei?
- Wie viel angeborene Struktur ist notwendig?
- Welche Mechanismen verursachen unnötige Komplexität?

---

# 9. Versuchsdesign und Vergleichsgruppen

Die vier Mechanismen dürfen nicht ausschließlich gemeinsam getestet werden.

Verwende einen aufeinander aufbauenden Versuchsplan.

| Konfiguration | Grundverschaltung | Exploration | Motorische Grundschaltung | Eigenrückmeldung |
|---|---|---|---|---|
| Referenz A | Nein | Nein | Nein | Nein |
| Stufe I | Ja | Nein | Nein | Nein |
| Stufe II | Ja | Ja | Nein | Nein |
| Stufe III | Ja | Ja | Ja | Nein |
| Stufe IV | Ja | Ja | Ja | Ja |

Ergänze gezielte Kontrollversuche, in denen jeweils ein bereits eingeführter Mechanismus wieder deaktiviert wird.

Dadurch lässt sich untersuchen, ob bestimmte beobachtete Fähigkeiten tatsächlich von diesem Mechanismus abhängen.

Die Ergebnisse der einzelnen Stufen sind jeweils zu dokumentieren, bevor die nächste Stufe ergänzt wird.

---

# 10. Netzwerk und Versuchsparameter

## 10.1 Ausgangsgröße

Verwende zunächst:

- 256 Neuronen.
- 1.024 reguläre initiale Synapsen.
- Virtuelle Geometrie 8 × 8 × 4.
- Bestehende neuronale Membran- und Feuermechanismen.
- Die vorhandene motorische Ausgabeschnittstelle.

Zusätzliche angeborene Synapsen sind separat zu zählen.

## 10.2 Initiale synaptische Gewichtungen

Die bisherige Gewichtung 0 bleibt als Referenz erhalten.

Für die funktionale Grundverschaltung dürfen niedrige positive Anfangsgewichte untersucht werden.

Diese müssen als angeborene Versuchsparameter dokumentiert sein.

Sie dürfen keine vorgegebene Antwortcodierung erzeugen.

## 10.3 Regulationsmechanismen

Berücksichtige die Ergebnisse der Experimente 002 und 003.

Als vergleichbare Konfigurationen können insbesondere dienen:

- Der ursprüngliche neuronale Kern.
- Die Refraktärvariante C5.
- C5 mit maximalem Synapsengewicht 1.

Dabei ist zu berücksichtigen, dass C5+A1 zwar längere Aktivität ermöglicht, aber langfristig weitgehend synaptisch sättigt.

Keine dieser Varianten gilt bereits als endgültige Architekturentscheidung.

Neue Lern- oder Regulationsmechanismen dürfen nicht eigenmächtig hinzugefügt werden.

## 10.4 Zufallsstartwerte

Verwende mehrere fest dokumentierte Seeds.

Mindestens die bereits etablierten Werte:

`20261117`, `7`, `42`, `99`, `2026`, `314159`

Die verschiedenen Formen von Zufall für Netzwerkentwicklung, Grundverschaltung und Exploration sollen unabhängig reproduzierbar sein.

---

# 11. Beobachtung und Messung

Erfasse über alle Forschungsphasen hinweg mindestens:

- Anzahl Neuronen und Synapsen.
- Anzahl angeborener Zusatzverbindungen.
- Anzahl strukturell erreichbarer motorischer Kontakte.
- Anzahl funktional aktivierter motorischer Kontakte.
- Anzahl explorativer Feuerereignisse.
- Anzahl neuronaler Gesamtfeuerereignisse.
- Anzahl vollständiger Antworten.
- Anzahl nicht abgeschlossener Antworten.
- Anzahl technischer Abbrüche.
- Verteilung ausgegebener Bitmuster.
- Motorische Aktivierungsreihenfolge.
- Anzahl sensorischer Eigenrückmeldungen.
- Aktivitätsveränderungen durch Rückkopplung.
- Maximale Ereigniswarteschlange.
- Synaptische Gewichtsverteilung.
- Speicherverbrauch.
- Rechenzeit.
- Synaptische Ereignisse pro Sekunde.

Die Messungen müssen zwischen externer Stimulation, Exploration, regulärer neuronaler Aktivität, motorischer Ausgabe und sensorischer Rückmeldung unterscheiden können.

---

# 12. Performance und Ressourcen

Die bisherigen Experimente zeigen, dass der neuronale Kern auf der vorhandenen CPU sehr effizient arbeitet.

Die neuen Mechanismen dürfen diese Eigenschaft nicht unnötig beeinträchtigen.

Insbesondere:

- Angeborene Verbindungen sollen normale kompakte Synapsenstrukturen verwenden.
- Exploration soll nur tatsächlich benötigte Ereignisse erzeugen.
- Motorische Grundschaltungen sollen möglichst ohne umfangreiche zusätzliche Verwaltungsdaten auskommen.
- Eigenrückmeldungen sollen über vorhandene Ereignismechanismen laufen.
- Keine vollständigen Netzwerkdurchläufe für einzelne motorische Ereignisse.
- Keine individuellen Zeitstempel pro Synapse.

Die zusätzlichen Kosten jedes Mechanismus sind getrennt zu messen.

Führe die Performance-Messungen sowohl mit deaktiviertem Lernen als auch mit den vorhandenen Lernregeln durch, soweit stabile Versuchsbedingungen möglich sind.

---

# 13. Automatisierte Tests

Die bestehende Testsuite muss vollständig bestehen bleiben.

Ergänze für jede Forschungsphase eigene Tests.

## Phase I – Grundverschaltung

Prüfe:

- Deterministische Initialisierung.
- Korrekte Anzahl angeborener Verbindungen.
- Keine Selbstverbindungen oder Duplikate.
- Strukturelle Erreichbarkeit.
- Funktionale Signalweiterleitung.
- Konfigurierbare Anfangsgewichtungen.
- Unverändertes Referenzverhalten bei deaktivierter Grundverschaltung.

## Phase II – Exploration

Prüfe:

- Reproduzierbare Aktivierung.
- Begrenzte Ereignisanzahl.
- Trennung explorativer und regulärer Ereignisse.
- Keine direkte Erzeugung fertiger Antwortbytes.
- Korrekte Behandlung von Ereignislimits.
- Unverändertes Verhalten ohne Exploration.

## Phase III – Motorische Grundschaltungen

Prüfe:

- Korrekte Aktivierung von Abschlusskontakten.
- Tatsächlich neuronalen Ursprung des Abschlusses.
- Unterscheidung von Abschluss und technischem Abbruch.
- Gültige Nullbyte-Ausgabe.
- Keine direkte Eingabe-Ausgabe-Kopie.
- Reproduzierbare motorische Schaltungen.

## Phase IV – Eigenrückmeldung

Prüfe:

- Rückmeldung nur nach tatsächlich beobachteter Motoraktivität.
- Korrekte Zuordnung zum auslösenden motorischen Ereignis.
- Trennung von Eigen- und Umweltrückmeldung.
- Korrekte sensorische Verarbeitung.
- Reproduzierbare Ereignisfolgen.
- Kontrollierter Abbruch bei Rückkopplungskaskaden.
- Unverändertes Verhalten bei deaktivierter Eigenrückmeldung.

## Integration

Prüfe zusätzlich:

- Zusammenspiel aller vier Mechanismen.
- Reproduzierbarkeit vollständiger Versuchsabläufe.
- Persistenz und Wiederherstellung.
- Stabilität der Ereignisverarbeitung.
- Unveränderte neuronale Grundregeln bei deaktivierten Erweiterungen.

---

# 14. Forschungsgrenzen

Nicht Bestandteil dieses Auftrags sind:

- Zielbewertung von Antworten.
- Belohnung oder Bestrafung.
- Vorhersagefehler.
- Erwartungsbildung.
- Rückmeldungsabhängige synaptische Lernregeln.
- Eligibility Traces.
- Zusätzliche lokale Ereignisgedächtnisse für synaptische Plastizität.
- Neue STDP-Verfahren.
- Synaptische Abschwächung.
- Neuartige Mechanismen für Synapsenschwund.
- Sprachverständnis.
- Vorgegebene Wörter oder Bedeutungen.
- Community-Lernen.

Die vier untersuchten Mechanismen sollen zunächst eine funktionierende angeborene sensomotorische Grundlage schaffen.

Gezieltes Lernen durch Bewertung folgt anschließend.

---

# 15. Arbeitsweise und Reihenfolge

Die Umsetzung erfolgt ausdrücklich in vier aufeinander aufbauenden Forschungsphasen.

## Schritt 1

Implementiere und untersuche die angeborene Grundverschaltung.

Dokumentiere deren Einfluss auf strukturelle und funktionale Erreichbarkeit.

## Schritt 2

Ergänze die explorative neuronale Aktivität.

Vergleiche die Ergebnisse mit und ohne angeborene Grundverschaltung.

## Schritt 3

Ergänze motorische Grundschaltungen.

Untersuche die Erzeugung vollständiger neuronaler Antworten.

## Schritt 4

Ergänze die sensorische Eigenrückmeldung.

Untersuche den geschlossenen sensomotorischen Ereigniskreislauf.

## Schritt 5

Führe einen integrierten Vergleich aller vier Mechanismen durch.

Jede Phase muss einen separat nachvollziehbaren Zwischenstand besitzen.

Fehlende Erfolge dürfen nicht durch undokumentierte zusätzliche Mechanismen ausgeglichen werden.

Wenn eine Phase keine funktionierenden Ergebnisse liefert, dokumentiere den Befund und die Auswirkungen auf die nachfolgenden Untersuchungen. Notwendige technische Anpassungen müssen kenntlich gemacht werden.

---

# 16. Dokumentation und Lieferumfang

Erstelle im Repository:

1. Einen übergeordneten Forschungsbericht für A.1.
2. Separate Ergebnisabschnitte für die vier Forschungsphasen.
3. Reproduzierbare Versuchsskripte.
4. Eine vollständige Dokumentation der experimentellen Parameter.
5. Maschinenlesbare Rohdaten.
6. Vergleichstabellen für Einzel- und Kombinationsversuche.
7. Automatisierte Tests.
8. Eine Übersicht aller vorgenommenen technischen Änderungen.
9. Eine Entscheidungsvorlage für die nachfolgende Stammhirnentwicklung.

Vorhandene Forschungsberichte und Rohdaten dürfen nicht überschrieben werden.

Bereits bestehende lokale Änderungen müssen erhalten bleiben.

Die vier Zwischenstände sollen durch nachvollziehbare Commits beziehungsweise Entwicklungsstände voneinander unterscheidbar sein.

Eine endgültige Architekturentscheidung erfolgt erst nach unserer gemeinsamen Auswertung.

---

# 17. Abschlusskriterien

Die Forschungsstufe gilt als abgeschlossen, wenn überprüfbar festgestellt werden kann:

- Ob eine angeborene Grundverschaltung die funktionale Erreichbarkeit der motorischen Kontakte verbessert.
- Ob explorative Aktivität zu unterschiedlichen neuronalen Ausgangsmustern führt.
- Ob motorische Grundschaltungen vollständige Antworten ermöglichen.
- Ob EVE ihre eigene motorische Aktivität als sensorische Erfahrung zurückerhält.
- Ob das Zusammenspiel dieser Mechanismen reproduzierbar und stabil ist.
- Welchen Speicher- und Rechenaufwand die Mechanismen verursachen.
- Welche Mechanismen für die weitere Entwicklung des Stammhirns notwendig erscheinen.

Ein negatives Ergebnis ist zulässig und vollständig zu dokumentieren.

Das Erreichen dieser Kriterien ist kein Nachweis für erfolgreiches Lernen oder Sprachfähigkeit.

---

# 18. Übergang zur nächsten Forschungsstufe

Nach Abschluss von A.1 soll EVE im Idealfall einen vollständigen sensomotorischen Kreislauf besitzen:

**Reiz → neuronale Verarbeitung → Exploration → motorische Handlung → Abschluss → sensorische Eigenrückmeldung.**

Darauf aufbauend kann die nächste Forschungsstufe die lokale neuronale Ereigniszuordnung untersuchen.

Erst anschließend wird eine bewertende Rückmeldung eingeführt.

Dann soll EVE nicht nur eigene Handlungen hervorbringen, sondern durch die beobachteten Auswirkungen dieser Handlungen auch ihre neuronalen Verbindungen verändern können.

---

## Zentraler Grundsatz

**Wir geben EVE keine angeborenen Antworten. Wir geben ihr ein angeborenes Nervensystem, das Wahrnehmung, Aktivität, Handlung und die Erfahrung ihrer eigenen Handlungen überhaupt erst möglich macht.**

Die Fähigkeiten, dieses Nervensystem sinnvoll zu nutzen, soll EVE durch ihre eigenen neuronalen Lernmechanismen entwickeln.
