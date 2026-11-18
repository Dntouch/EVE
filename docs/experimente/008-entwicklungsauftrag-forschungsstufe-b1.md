# EVE – Forschungsstufe B.1
## Eigenständige neuronale Erholung und wiederholte Handlungsfähigkeit

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-b-local-event-memory`  
**Status:** Forschungs- und Entwicklungsauftrag  
**Implementierung:** C++17

---

# 1. Ausgangssituation

Die bisherigen Forschungsstufen haben wesentliche Grundlagen eines biologisch inspirierten neuronalen Systems geschaffen.

Vorhanden sind:

- Ein ereignisgesteuerter neuronaler Kern.
- Gerichtete Synapsen und veränderliche Eingangsgewichte.
- Neuronale Membranpotenziale und Feuerentscheidungen.
- Eine reproduzierbare Netzwerkinitialisierung.
- Angeborene sensorische und motorische Grundverschaltungen.
- Explorative neuronale Aktivierung.
- Die motorische Akkumulatorschaltung M3.
- Eine technische sensorische Eigenrückmeldung.
- Ein lokales neuronales Ereignisgedächtnis.
- Vollständige neuronale Persistenz.

Forschungsstufe A.2 hat nachgewiesen, dass die Schaltung M3 eine motorische Handlung über einen tatsächlichen neuronalen Signalweg abschließen kann.

Forschungsstufe B hat anschließend gezeigt, dass ein lokales Ereignisgedächtnis die an solchen Handlungen beteiligten Eingangssynapsen erfassen kann.

In 64 untersuchten Netzwerkinitialisierungen wurden 14 kausal nachvollziehbare M3-Handlungswege festgestellt.

Die Speichervarianten G2 bis G4 konnten alle 14 Wege anhand lokaler Eingangsinformationen rekonstruieren.

Für weitere Untersuchungen wurde G3 mit acht Ereigniseinträgen pro Neuron als vorläufiger Kandidat ausgewählt.

## 1.1 Das verbleibende Problem

EVE kann eine Handlung ausführen, aber unter den bisherigen Versuchsbedingungen häufig nicht zuverlässig wiederholen.

Bei identisch wiederhergestellten Ausgangszuständen können dieselben neuronalen Reaktionen reproduziert werden.

Werden dagegen mehrere Antwortphasen mit fortgeführtem Netzwerkzustand ausgeführt, verschwindet die motorische Aktivität häufig nach der ersten Handlung.

Forschungsstufe B dokumentierte bei fortgeführten Versuchen außerdem deutlich mehr refraktär unterdrückte Feuerentscheidungen als bei wiederhergestellten Zuständen.

Die aktuelle Refraktärregel ist ein möglicher wesentlicher Verursacher.

## 1.2 Bisherige Refraktärregel

Nach einem Feuerereignis wird ein Neuron für 16 eingehende Impulse refraktär.

Während dieser Phase:

- Empfängt das Neuron weiterhin Signale.
- Werden synaptische Eingänge verarbeitet.
- Kann das Membranpotenzial weiter anwachsen.
- Bleibt erneutes Feuern unterdrückt.
- Wird der Refraktärzähler ausschließlich durch weitere eingehende Impulse reduziert.

Empfängt das Neuron keine weiteren Signale, kann seine Refraktärphase unbegrenzt bestehen bleiben.

Das unterscheidet sich grundlegend von der biologischen Erholung neuronaler Membranen.

**Wir vermuten deshalb, dass unsere gegenwärtige Refraktärregel die erneute Aktivierbarkeit von Neuronen ungewollt an weitere Netzaktivität koppelt.**

Diese Hypothese soll untersucht werden.

---

# 2. Zentrale Forschungsfrage

**Kann eine eigenständige, ereignisgesteuerte neuronale Erholung die wiederholte Handlungsfähigkeit von EVE verbessern, ohne synaptische Gewichtungen, Verbindungen oder lokale Erinnerungen künstlich zurückzusetzen?**

Zusätzlich soll geklärt werden:

1. Welchen Anteil hat die bisherige Refraktärregel an der fehlenden Wiederholbarkeit?
2. Welche Rolle spielt das während der Refraktärphase akkumulierte Membranpotenzial?
3. Wie verändert sich die neuronale Aktivität durch eine unabhängige Erholung?
4. Können wiederholte M3-Handlungen mit fortgeführtem Zustand entstehen?
5. Bleiben zuvor gespeicherte neuronale Informationen erhalten?
6. Wie hoch ist der zusätzliche Rechen- und Speicheraufwand?

---

# 3. Grundsatzentscheidung

Für EVEs weitere Entwicklung wird folgender Grundsatz festgelegt:

**Ein Neuron muss sich nach dem Feuern unabhängig davon erholen können, ob es weitere Eingangssignale erhält.**

Dabei unterscheiden wir:

### Kurzfristige neuronale Zustände

- Membranpotenzial.
- Erregbarkeit.
- Refraktärzustand.

### Längerfristige neuronale Zustände

- Synaptische Gewichtungen.
- Bestehende Verbindungen.
- Lokal gespeicherte Eingangsereignisse.
- Erlernte neuronale Strukturen.

Die Wiederherstellung neuronaler Erregbarkeit darf die längerfristigen Zustände nicht löschen.

Eine Änderung des Membranpotenzials ist ausdrücklich keine synaptische Abschwächung und kein Vergessen.

---

# 4. Technische Architekturprinzipien

Beizubehalten sind:

- C++17.
- Trennung zwischen BIOS, Stammhirn und neuronalem Netzwerk.
- Ereignisgesteuerte Verarbeitung.
- Keine globalen neuronalen Ticks.
- Keine periodischen vollständigen Netzwerkdurchläufe.
- Keine Zeitstempel pro Synapse.
- Keine regelmäßige Aktualisierung sämtlicher Neuronen.
- Ein ungewichteter logischer Ausgang pro Neuron.
- Individuell gewichtete Eingänge.
- Reproduzierbare Zufallszustände.
- Kompakte Speicherstrukturen.
- Persistenz vollständiger neuronaler Zustände.

Die bisherige FIFO-basierte Ereignisverarbeitung darf um eine geordnete zukünftige Ereignisplanung erweitert werden.

Diese Erweiterung soll möglichst unabhängig vom eigentlichen neuronalen Kern bleiben.

**Die technische Zeitverwaltung ist Aufgabe des BIOS. Die neuronalen Erholungsregeln gehören zum Stammhirn.**

---

# 5. Forschungsphase I – Bestehende Refraktärprobleme analysieren

## 5.1 Ziel

Vor Implementierung einer neuen Erholungsregel soll der bisherige Mechanismus gezielt vermessen werden.

Es muss festgestellt werden, warum ein Neuron in einer folgenden Antwortphase nicht erneut feuert.

## 5.2 Versuchsgrundlage

Verwende die bisherige M3-Grundkonfiguration:

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| Initiale reguläre Synapsen | 1.024 |
| Geometrie | 8 × 8 × 4 |
| Angeborene Verschaltung | V2 |
| Angeborenes Gewicht | 3 |
| Motorische Schaltung | M3 |
| Feuergrenze | 3 |
| Refraktärereignisse | 16 |
| Lokales Ereignisgedächtnis | G3, 8 Einträge |
| Lernen | Deaktiviert |
| Wachstum | Deaktiviert |
| Eingeschränkte Exploration | E4-neu |

Die konkreten M3-Gewichte werden aus der bestehenden Forschungsstufe übernommen.

## 5.3 Zu untersuchende Neuronen

Erfasse insbesondere:

- Sensorische Eingangsneuronen.
- Das Neuron des Antwortreizes.
- An der M3-Handlung beteiligte Datenmotorneuronen.
- Den M3-Akkumulator.
- Das motorische Abschlussneuron.
- Beteilige Relais der angeborenen Grundverschaltung.

## 5.4 Messwerte

Für jedes untersuchte Neuron:

- Membranpotenzial vor und nach dem Feuern.
- Refraktärzustand nach dem Feuern.
- Refraktärzustand zu Beginn der nächsten Antwortphase.
- Anzahl zwischenzeitlich empfangener Impulse.
- Anzahl unterdrückter Feuerentscheidungen.
- Anzahl tatsächlich erfolgter Feuerereignisse.
- Verbleibender Refraktärzustand nach einer weiteren Antwortaufforderung.

Zusätzlich:

- Vollständige motorische Aktivitätsfolge.
- Abschlussstatus.
- Größe der Ereigniswarteschlange.
- Zustand des lokalen Ereignisgedächtnisses.

## 5.5 Abschluss dieser Phase

Erstelle eine Diagnose, welche Neuronen eine zweite Handlung verhindern und welchen Anteil die Refraktärphase daran hat.

Andere Ursachen sind ausdrücklich zu dokumentieren.

Die neue Erholungsregel darf nicht als Lösung vorausgesetzt werden.

---

# 6. Forschungsphase II – Ereignisgesteuerte Zeitplanung

## 6.1 Ziel

Erweitere die technische Ereignisverarbeitung so, dass zukünftige neuronale Ereignisse geplant werden können.

Die Zeitplanung muss ohne globale neuronale Aktualisierungsdurchläufe funktionieren.

## 6.2 Simulierte Zeit

Für die ersten Experimente wird eine deterministische, simulierte Zeit verwendet.

Diese wird als ganzzahliger Wert dargestellt, beispielsweise durch einen 64-Bit-Zähler.

Eine Zeiteinheit ist zunächst eine abstrakte Simulationseinheit.

Sie entspricht nicht automatisch einer Millisekunde oder einer anderen physikalischen Einheit.

Die Abbildung auf reale Zeit kann später untersucht werden.

## 6.3 Geplante Ereignisse

Die Ereignisverwaltung muss mindestens unterscheiden:

- Sofortige neuronale Feuerereignisse.
- Synaptische Impulsereignisse.
- Geplante Erholungsereignisse.
- Technische Ereignisse für Versuchsgrenzen.

Ein Erholungsereignis besitzt:

- Die ID des betroffenen Neurons.
- Den geplanten Ausführungszeitpunkt.
- Falls erforderlich eine eindeutige Kennung zur Vermeidung veralteter oder doppelter Erholungsereignisse.

Diese Informationen gehören zur technischen Ereignisverwaltung.

**Es werden keine Zeitstempel in einzelnen Synapsen gespeichert.**

## 6.4 Ereignisreihenfolge

Für Ereignisse mit identischem Ausführungszeitpunkt muss eine deterministische Verarbeitungsreihenfolge gelten.

Die bisherige Reihenfolge unmittelbarer neuronaler Ereignisse soll möglichst erhalten bleiben.

Änderungen der Ereignisreihenfolge müssen dokumentiert und kontrolliert verglichen werden.

Insbesondere ist zu vermeiden, dass allein die Einführung der Zeitplanung bereits das neuronale Verhalten des Referenzmodells verändert.

## 6.5 Zeitfortschritt

Die simulierte Zeit darf auch ohne neue sensorische Eingänge fortschreiten, sofern ein geplantes zukünftiges Ereignis verarbeitet werden soll.

Wichtig ist jedoch:

**Das Ende einer Antwortphase darf nicht automatisch dazu führen, dass sämtliche zukünftigen Erholungsereignisse sofort abgearbeitet werden.**

Andernfalls würden wir zwischen Antwortphasen künstlich eine vollständige Erholung erzeugen.

Es muss ausdrücklich steuerbar sein:

- Bis zu welchem simulierten Zeitpunkt Ereignisse verarbeitet werden.
- Wann eine Antwortphase beginnt.
- Wann eine Antwortphase endet.
- Wann ein neuer externer Reiz eintrifft.
- Wie viel simulierte Zeit zwischen zwei Handlungen vergeht.

## 6.6 Persistenz

Die vollständige Zeitplanung muss speicherbar und wiederherstellbar sein.

Dazu gehören:

- Aktuelle simulierte Zeit.
- Ausstehende geplante Ereignisse.
- Deren Ausführungsreihenfolge.
- Zugehörige neuronale Erholungszustände.

Nach Wiederherstellung muss das System dieselben Ereignisse in derselben Reihenfolge verarbeiten.

## 6.7 Technische Tests

Prüfe insbesondere:

- Ereignisplanung in der Zukunft.
- Verarbeitung mehrerer Ereignisse mit gleichem Zeitpunkt.
- Deterministische Ereignisreihenfolge.
- Korrekte Verarbeitung ohne weitere Eingänge.
- Sicherer Umgang mit überholten Erholungsereignissen.
- Vermeidung doppelter gültiger Erholungsereignisse.
- Persistenz während einer ausstehenden Erholung.
- Korrekte Fortsetzung nach Wiederherstellung.
- Kein globaler neuronaler Tick.
- Kein vollständiger Netzwerkdurchlauf für einzelne Erholungsereignisse.

---

# 7. Forschungsphase III – Neue neuronale Erholungsmechanismen

Untersuche drei voneinander unterscheidbare Varianten.

## R0 – Bisherige ereignisabhängige Refraktärregel

Dies ist die unveränderte Referenz.

Nach dem Feuern bleibt ein Neuron für 16 weitere eingehende Impulse refraktär.

Es gibt kein eigenständiges Erholungsereignis.

Die ursprünglichen Membran- und Reset-Regeln bleiben erhalten.

## R1 – Eigenständige neuronale Erholung

Nach einem Feuerereignis wird ein zukünftiges Erholungsereignis geplant.

Bis zu diesem Ereignis ist das Neuron refraktär.

Beim Erholungsereignis:

1. Wird die Refraktärphase beendet.
2. Wird das Neuron wieder erregbar.
3. Bleiben sämtliche Synapsengewichte unverändert.
4. Bleiben sämtliche Synapsenverbindungen unverändert.
5. Bleibt das lokale Ereignisgedächtnis unverändert.
6. Wird kein neues Feuerereignis erzeugt.

Während der Refraktärphase werden eingehende Impulse weiterhin verarbeitet.

Die bisherigen Lernzähler bleiben funktionsfähig.

Das Membranpotenzial wird in R1 weiterhin entsprechend dem bestehenden Modell behandelt. Eine zusätzliche Membranentspannung findet nicht statt.

Damit untersuchen wir zunächst ausschließlich die neue Erholung der Erregbarkeit.

## R2 – Eigenständige Erholung mit Membranentspannung

R2 erweitert R1 um eine Veränderung des kurzfristigen Membranpotenzials.

Biologisch nähert sich das Membranpotenzial nach neuronaler Aktivität wieder einem Ruhewert.

Für EVE soll untersucht werden, ob eine solche Erholung zusätzlich zur Refraktärregel erforderlich ist.

Dabei gilt:

- Synaptische Gewichtungen bleiben unverändert.
- Synapsenverbindungen bleiben unverändert.
- Lokale Ereignisgedächtnisse bleiben erhalten.
- Es werden keine neuen Lernmechanismen eingeführt.
- Das Membranpotenzial darf sich in Richtung eines definierten Ruhewerts verändern.

### Experimentelle Umsetzung

Untersuche zunächst eine klar definierte, ereignisgesteuerte Membranentspannung beim Erholungsereignis.

Beispielsweise:

- Keine Membranentspannung – entspricht R1.
- Teilweise Annäherung an das Ruhepotenzial.
- Vollständige Rückkehr zum Ruhepotenzial.

Die genauen Faktoren sind konfigurierbare Versuchsparameter.

Diese einmalige Entspannung ist noch kein kontinuierliches biologisches Membranleck.

Ein kontinuierlicher zeitabhängiger Mechanismus kann später gesondert untersucht werden.

### Wichtig

Die R2-Variante darf nicht automatisch mit einem zusätzlichen Synapsen- oder Gedächtnisreset kombiniert werden.

Nur der kurzfristige Membranzustand darf verändert werden.

---

# 8. Forschungsphase IV – Erholungsdauer untersuchen

## 8.1 Ziel

Ermittle, welchen Einfluss die Dauer der eigenständigen Refraktärphase auf neuronales Verhalten und Wiederholbarkeit besitzt.

## 8.2 Experimentelle Parameter

Für R1 und R2 sollen zunächst mehrere simulierte Erholungsdauern untersucht werden.

Vorgeschlagene Werte:

| Variante | Erholungsdauer |
|---|---:|
| T1 | 1 Simulationseinheit |
| T2 | 4 Simulationseinheiten |
| T3 | 16 Simulationseinheiten |
| T4 | 64 Simulationseinheiten |

Die Werte sind technische Ausgangsparameter und keine biologisch begründeten Zeitkonstanten.

## 8.3 Zwischenräume zwischen Handlungen

Untersuche unterschiedliche Abstände zwischen zwei Antwortaufforderungen.

Beispielsweise:

- Keine zusätzliche Wartephase.
- Kürzer als die Erholungsdauer.
- Gleich der Erholungsdauer.
- Doppelte Erholungsdauer.
- Deutlich länger als die Erholungsdauer.

Dabei muss sichergestellt sein, dass die simulierte Zeit tatsächlich fortschreitet.

R0 erhält dieselben äußeren zeitlichen Versuchsbedingungen als Kontrollgruppe, auch wenn seine Refraktärregel selbst nicht zeitabhängig ist.

## 8.4 Messgrößen

- Anteil erholter Neuronen vor der nächsten Antwort.
- Anzahl erneut feuernder Neuronen.
- Anzahl abgeschlossener M3-Handlungen.
- Anzahl refraktär unterdrückter Feuerentscheidungen.
- Membranpotenziale vor neuer Stimulation.
- Aktivitätsverteilung.
- Gesamtereignisse.
- Antwortlatenz in Simulationseinheiten.

## 8.5 Entscheidende Frage

**Kann ein Neuron nach einer ausreichenden Erholungszeit erneut feuern, obwohl es während seiner Refraktärphase keine zusätzlichen Impulse empfangen hat?**

Diese Eigenschaft ist für R1 und R2 verbindlich nachzuweisen.

---

# 9. Forschungsphase V – Wiederholte neuronale Handlungen

## 9.1 Ziel

Dies ist die zentrale experimentelle Untersuchung.

EVE soll mehrere Handlungen nacheinander ausführen können, ohne dass der neuronale Ausgangszustand vor jeder Handlung wiederhergestellt wird.

## 9.2 Versuchsaufbau

Verwende die bisherige M3-Schaltung.

Die neuronale Ausgangsaktivität soll weiterhin über die regulären sensorischen und motorischen Signalwege entstehen.

Keine direkte motorische Aktivierung als Ersatz für die Netzwerkverarbeitung.

## 9.3 Versuchsablauf

1. Initialisiere das Netzwerk einmalig.
2. Präsentiere `AB` als Eingabe.
3. Erzeuge einen Antwortreiz.
4. Erfasse die neuronale Handlung.
5. Beobachte den Antwortabschluss.
6. Verarbeite die vorgesehene Erholungsphase.
7. Präsentiere erneut `AB`.
8. Wiederhole den Ablauf ohne Wiederherstellung des ursprünglichen Netzwerkzustands.

## 9.4 Wiederholungszahlen

Untersuche mindestens:

- 2 Antwortphasen.
- 8 Antwortphasen.
- 32 Antwortphasen.
- 128 Antwortphasen.

Die längeren Versuche erfolgen nur, wenn das Netzwerk unter den gewählten Bedingungen ausreichend stabil bleibt.

Jede Antwortphase muss separat ausgewertet werden.

## 9.5 Zusätzliche Eingabefolgen

Untersuche auch:

- `AB`
- `BA`
- `AA`
- `BB`
- Wechselnde Eingaben `AB`, `BA`, `AB`, `BA`

Dadurch soll geprüft werden, ob die Wiederholbarkeit von einer einzelnen Eingabefolge abhängt.

## 9.6 Bewertung

Erfasse:

- Anzahl aller Antwortversuche.
- Anzahl gültig abgeschlossener Antworten.
- Anzahl Abschlüsse über einen nachgewiesenen M3-Weg.
- Anzahl zufälliger oder nicht zuordenbarer Abschlüsse.
- Anzahl Antwortphasen mit Datenmotoraktivität.
- Anzahl Antwortphasen ohne motorische Aktivität.
- Veränderung der neuronalen Zustände zwischen den Versuchen.
- Fortbestehende Refraktärzustände.
- Veränderung der Membranpotenziale.
- Aktivität der angeborenen sensorischen und motorischen Schaltungen.

## 9.7 Vergleichsgruppen

**K0 – Wiederhergestellter Zustand**

Jede Handlung beginnt mit demselben gespeicherten Ausgangszustand.

Diese Variante dient ausschließlich der Referenz.

**K1 – Fortgeführter Zustand mit R0**

Bisherige ereignisabhängige Refraktärregel.

**K2 – Fortgeführter Zustand mit R1**

Eigenständige neuronale Erholung.

**K3 – Fortgeführter Zustand mit R2**

Eigenständige Erholung und Membranentspannung.

Die zugrunde liegenden Netzwerke müssen identisch initialisiert werden.

## 9.8 Erfolgskriterien

Eine Verbesserung liegt vor, wenn R1 oder R2 unter identischen Versuchsbedingungen zuverlässig mehr kausal nachvollziehbare Handlungen ermöglicht als R0.

Eine höhere Anzahl zufälliger Abschlussereignisse genügt nicht.

Das Netzwerk muss weiterhin unterscheidbare motorische Aktivitäten erzeugen können.

---

# 10. Forschungsphase VI – Erhaltung neuronaler Erinnerungen

## 10.1 Ziel

Prüfe ausdrücklich, ob die neue Erholung ausschließlich kurzfristige neuronale Zustände verändert.

## 10.2 Synaptische Zustände

Erfasse vor und nach einer Erholungsphase:

- Anzahl der Synapsen.
- IDs und Endpunkte der Synapsen.
- Synaptische Gewichtungen.
- Synaptische Lernzähler.

Bei deaktiviertem Lernen und Wachstum dürfen durch die Erholung keine Synapsen verändert oder entfernt werden.

## 10.3 Lokales Ereignisgedächtnis

Verwende G3 mit acht gespeicherten Eingangssynapsen pro Neuron.

Prüfe:

- Bleiben die gespeicherten Synapsen-IDs erhalten?
- Bleibt die Eingangsreihenfolge erhalten?
- Bleiben die Verwaltungszustände des Rings erhalten?
- Bleiben die Informationen über eine frühere M3-Handlung erhalten, solange keine neuen Eingänge sie überschreiben?

Die Erholung darf das Ereignisgedächtnis nicht löschen.

Wenn neue Eingangsereignisse Einträge überschreiben, ist dies gesondert auszuweisen und nicht als Folge des Erholungsmechanismus zu klassifizieren.

## 10.4 Persistenz

Speichere das Netzwerk:

- Unmittelbar nach einer Handlung.
- Während einer laufenden neuronalen Erholungsphase.
- Nach erfolgter Erholung.
- Vor einer weiteren Antwortaufforderung.

Prüfe, ob nach Wiederherstellung dasselbe neuronale Verhalten entsteht.

Dabei müssen auch zukünftige geplante Erholungsereignisse korrekt erhalten bleiben.

---

# 11. Forschungsphase VII – Eigenrückmeldung und fortlaufender Betrieb

## 11.1 Ziel

Untersuche, ob die sensorische Eigenrückmeldung aus Forschungsstufe A.1 mit den neuen Erholungsmechanismen zusammenarbeitet.

## 11.2 Kontrollvarianten

**F0 – Keine Eigenrückmeldung**

Nur sensorische Eingabe, neuronale Handlung und Erholung.

**F1 – Eigenrückmeldung nach Abschluss**

Die tatsächlich beobachtete motorische Ausgabe wird nach einem gültigen Abschluss sensorisch zurückgeführt.

**F2 – Rückmeldung einzelner Motorereignisse**

Motorische Ereignisse werden in ihrer ursprünglichen Reihenfolge sensorisch zurückgeführt.

## 11.3 Forschungsfragen

- Kann EVE nach der Eigenrückmeldung erneut handeln?
- Verändert die zusätzliche sensorische Aktivität den Erholungsverlauf?
- Werden weitere Neuronen refraktär?
- Entstehen selbstverstärkende Rückkopplungsschleifen?
- Verändert sich die Anzahl gültiger M3-Abschlüsse?
- Bleibt die lokale Ereigniserinnerung erhalten?
- Ist das Verhalten über mehrere Antwortphasen reproduzierbar?

Die tatsächliche Eigenrückmeldung bleibt von einer Erfolgsbewertung getrennt.

Es wird noch keine Belohnung eingeführt.

---

# 12. Reproduzierbarkeit und Stichprobe

## 12.1 Zufallsstartwerte

Verwende mindestens alle 64 Seeds aus Forschungsstufe B.

Die aus Forschungsstufe A.2 bekannten erfolgreichen und erfolglosen Seeds müssen enthalten sein.

Weitere Seeds dürfen ergänzt werden.

## 12.2 Vergleichbarkeit

Für jede untersuchte Variante gelten möglichst identische:

- Ausgangsvernetzungen.
- Angeborene Pfade.
- M3-Schaltungen.
- Explorationsfolgen.
- Sensorische Eingaben.
- Ausgangsgewichtungen.
- Ereignislimits.
- Zeitliche Eingabeabstände.

Die Erholungsregel bildet die jeweils untersuchte Veränderung.

## 12.3 Statistische Auswertung

Berichte:

- Absolute Erfolgszahlen.
- Erfolgsanteile.
- Verteilungen über alle Seeds.
- Mittelwerte und Streuungen.
- Verteilung der Zahl abgeschlossener Wiederholungen.
- Häufigkeit technischer Abbrüche.
- Unterschiede zwischen einzelnen Erholungsdauern.

Einzelne besonders erfolgreiche Seeds dürfen nicht als repräsentatives Gesamtergebnis gewertet werden.

---

# 13. Performance und Ressourcenverbrauch

## 13.1 Ziel

Ermittle den zusätzlichen Ressourcenbedarf der ereignisgesteuerten Erholungsverwaltung.

## 13.2 Messwerte

Erfasse:

- Größe der zukünftigen Ereigniswarteschlange.
- Anzahl geplanter Erholungsereignisse.
- Anzahl verarbeiteter Erholungsereignisse.
- Anzahl gegebenenfalls ungültig gewordener Erholungsereignisse.
- Speicherbedarf der Zeitverwaltung.
- Zusätzliche Zustandsdaten pro Neuron.
- Laufzeit pro synaptischem Impuls.
- Laufzeit pro Feuerereignis.
- Laufzeit pro Erholungsereignis.
- Maximale Speicherbelegung.
- Synaptische Ereignisse pro Sekunde.

## 13.3 Netzwerkgrößen

Untersuche die technische Skalierung nach Möglichkeit mit:

| Neuronen | Initiale Synapsen |
|---:|---:|
| 256 | 1.024 |
| 1.024 | 4.096 |
| 10.000 | 40.000 |
| 100.000 | 400.000 |
| 1.000.000 | 4.000.000 |

Größere Netzwerke dürfen für reine Performanceuntersuchungen verwendet werden.

Sie sind nicht automatisch Bestandteil der vollständigen motorischen Versuchsreihe.

## 13.4 Wichtige Effizienzanforderung

Ein Erholungsereignis soll ausschließlich das betroffene Neuron bearbeiten.

Es darf keinen vollständigen Netzwerkdurchlauf auslösen.

Die Verwaltung soll auch bei sehr großen Netzwerken effizient funktionieren, wenn nur wenige Neuronen aktiv sind.

---

# 14. Automatisierte Tests

Ergänze die vorhandene Testsuite.

Mindestens erforderlich sind folgende Prüfungen:

### Zeitverwaltung

1. Deterministische simulierte Zeit.
2. Korrekte Planung zukünftiger Ereignisse.
3. Deterministische Verarbeitung bei identischen Zeitpunkten.
4. Korrekte Ereignisreihenfolge.
5. Verarbeitung eines Erholungsereignisses ohne neue Eingangsimpulse.
6. Sicherer Umgang mit veralteten Erholungsereignissen.
7. Keine unbeabsichtigte Vorverarbeitung zukünftiger Erholungen beim Antwortabschluss.
8. Persistenz ausstehender Ereignisse.

### Neuronale Erholung

9. Feuerereignis startet eine Refraktärphase.
10. R0 bleibt unverändert.
11. R1 beendet die Refraktärphase nach geplanter Erholung.
12. R1 verändert das Membranpotenzial nicht zusätzlich.
13. R2 verändert ausschließlich das Membranpotenzial gemäß Versuchsregel.
14. Erholung erzeugt kein eigenes Feuerereignis.
15. Ein erholtes Neuron kann bei erneutem geeignetem Eingang feuern.
16. Kein neuer Eingang ist für den Ablauf der Erholung erforderlich.

### Gedächtniserhaltung

17. Synapsengewichte bleiben erhalten.
18. Synapsenverbindungen bleiben erhalten.
19. Lokale Ereignisgedächtnisse bleiben erhalten.
20. Die Reihenfolge gespeicherter Ereignisse bleibt erhalten.
21. Erholung verursacht keine synaptischen Lernereignisse.

### Motorische Handlungen

22. Wiederholte M3-Handlungen können untersucht werden, ohne den Zustand wiederherzustellen.
23. Gültige Abschlüsse bleiben von zufälligen Abschlüssen unterscheidbar.
24. Fortgeführte und wiederhergestellte Zustände werden korrekt getrennt.
25. Die M3-Kausalitätsdiagnose funktioniert weiterhin.
26. Eigenrückmeldung bleibt funktionsfähig.
27. Ein technischer Abbruch wird nicht als neuronaler Abschluss gewertet.

### Allgemein

28. Identische Seeds und Eingaben liefern reproduzierbare Ergebnisse.
29. Die bestehende Persistenz bleibt abwärtskompatibel, soweit technisch möglich.
30. Die gesamte bisherige Testsuite besteht weiterhin.

---

# 15. Forschungsgrenzen

Nicht Bestandteil dieses Auftrags sind:

- Neue synaptische Lernregeln.
- Neue Wachstumsregeln.
- Neue hemmende Synapsen.
- Synaptische Abschwächung.
- Synapsenschwund als neuer Lernmechanismus.
- STDP.
- Eligibility Traces.
- Rückmeldungsabhängige Gewichtsveränderungen.
- Vorhersagefehler.
- Neue Bewertungssignale.
- Vorgegebene Handlungsmuster.
- Vorgegebene sprachliche Bedeutungen.
- Community-Lernen.
- Eine GPU-Implementierung.

Die vorhandenen synaptischen Lernregeln dürfen für gesonderte Kontrollversuche aktiviert werden.

Sie dürfen nicht verändert werden.

Die Hauptuntersuchung erfolgt zunächst ohne Lernen und Wachstum.

---

# 16. Dokumentation und Lieferumfang

Erstelle im Repository:

1. Einen ausführlichen Forschungsbericht B.1 als Markdown.
2. Eine technische Spezifikation der ereignisgesteuerten Zeitplanung.
3. Eine Dokumentation der drei Erholungsvarianten.
4. Reproduzierbare Versuchsskripte.
5. Maschinenlesbare Rohdaten.
6. Vergleichstabellen für R0, R1 und R2.
7. Eine Analyse der wiederholten M3-Handlungen.
8. Diagnosebeispiele einzelner Neuronen.
9. Messungen zum Erhalt synaptischer Zustände und lokaler Erinnerungen.
10. Performance- und Speichermessungen.
11. Erweiterte automatisierte Tests.
12. Eine Entscheidungsvorlage für die nächste Stammhirnentwicklung.

Alle experimentellen Parameter und Implementierungsannahmen sind zu dokumentieren.

Die vorhandenen Forschungsberichte und Rohdaten dürfen nicht überschrieben werden.

Bestehende lokale Änderungen sind zu erhalten.

---

# 17. Abschlusskriterien

Forschungsstufe B.1 soll folgende Fragen empirisch beantworten:

1. Verhindert die bisherige eingangsabhängige Refraktärphase wiederholte motorische Handlungen?
2. Ermöglicht eine unabhängige neuronale Erholung eine Verbesserung?
3. Welchen zusätzlichen Einfluss hat eine Membranentspannung?
4. Welche Erholungsdauern unterstützen wiederholte neuronale Aktivität?
5. Können vollständige M3-Handlungen aus fortgeführten Netzwerkzuständen entstehen?
6. Bleiben Synapsengewichte, Verbindungen und lokale Ereigniserinnerungen unverändert?
7. Funktioniert die Eigenrückmeldung weiterhin?
8. Wie stark steigt der Rechen- und Speicherverbrauch?
9. Welche Variante eignet sich als Grundlage für kontinuierliches neuronales Lernen?

Es ist ausdrücklich zulässig, dass keine der untersuchten Varianten eine ausreichend zuverlässige Wiederholung ermöglicht.

In diesem Fall müssen die verbleibenden Ursachen identifiziert werden.

Eine bloße Erhöhung der Abschlussrate ist kein ausreichender Erfolgsnachweis, wenn die Abschlüsse nicht mehr auf reguläre neuronale Handlungswege zurückzuführen sind.

---

# 18. Übergang zur rückmeldungsabhängigen Lernforschung

Nach erfolgreicher Untersuchung der neuronalen Erholung soll Forschungsstufe B.2 die zeitlich begrenzte lokale Lernberechtigung untersuchen.

Dafür stehen dann im Idealfall drei Voraussetzungen bereit:

1. Wiederholt ausführbare neuronale Handlungen.
2. Lokal gespeicherte Informationen über beteiligte Eingänge.
3. Eine unabhängige Beobachtung der tatsächlichen Handlung und ihrer Wirkung.

Erst darauf aufbauend soll untersucht werden, wie das Stammhirn eine positive oder negative Rückmeldung an zuvor aktive neuronale Verbindungen koppeln kann.

Das Ziel bleibt ein geschlossener Lernkreislauf:

**Wahrnehmung → Erwartung → Handlung → Ergebnis → Abweichung → neuronale Anpassung.**

Die Entwicklung der neuronalen Erwartungsbildung erfolgt in einer späteren, getrennt überprüfbaren Stufe.

---

## Zentraler Forschungsgrundsatz

**Neuronale Erholung darf nicht mit neuronaler Erinnerung verwechselt werden.**

Ein Neuron soll nach dem Feuern erneut erregbar werden können, ohne seine bisherigen Erfahrungen zu verlieren.

Die vorübergehende Aktivität eines Neurons und die langfristige Entwicklung seiner synaptischen Verbindungen sind unterschiedliche Vorgänge.

Forschungsstufe B.1 soll erstmals systematisch untersuchen, ob diese Trennung EVE eine fortlaufende, wiederholbare neuronale Handlungsfähigkeit ermöglicht.
