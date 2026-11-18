# EVE – Forschungsstufe B.3.1
## Neuronaler Handlungskontext und zuverlässige Zuordnung verzögerter Rückmeldungen

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-b3-neural-feedback-routing`  
**Status:** Forschungs- und Entwicklungsauftrag  
**Implementierung:** C++17

---

# 1. Ausgangssituation

EVE verfügt inzwischen über wesentliche Bestandteile eines künstlichen neuronalen Lernsystems.

Vorhanden sind:

- Ereignisgesteuerte neuronale Signalverarbeitung.
- Individuelle synaptische Eingangsgewichtungen.
- Angeborene sensorische und motorische Grundverschaltungen.
- Explorative neuronale Aktivität.
- Wiederholt ausführbare motorische Handlungen.
- Eigenständige neuronale Erholung R2F.
- Lokales neuronales Ereignisgedächtnis G3.
- Zeitlich begrenzte lokale Lernberechtigungen (*Eligibility*).
- Sensorische Eigenrückmeldung.
- Neuronale Übertragung abgestufter Erfolgsbewertungen.
- Deterministische Simulation und Persistenz.

Forschungsstufe B.3 hat nachgewiesen, dass Bewertungen über reguläre neuronale Wege übertragen werden können.

Dabei wurden vier Rückmeldungsvarianten untersucht:

- F0: Keine neuronale Rückmeldung.
- F1: Einzelner sensorischer Rückmeldungseingang.
- F2: Allgemeine neuronale Direktverteilung.
- F3: Rückmeldung über allgemeine neuronale Relais.

F3 erreichte in der untersuchten 64-Seed-Matrix 580 Empfänge mit bereits vorhandener Eligibility.

Gleichzeitig wurden 8.199 Empfänge ohne vorherige Eligibility sowie 1.461 neu durch Rückmeldung erzeugte Eligibility-Entscheidungen beobachtet.

Damit ist die neuronale Übertragung nachgewiesen, eine korrekte Handlungszuordnung jedoch nicht.

## 1.1 Das ungelöste Problem

EVE kann mehrere Handlungen nacheinander ausführen.

Jede Handlung kann neuronale Aktivität und lokale Eligibility-Markierungen erzeugen.

Trifft eine Bewertung später ein, können Markierungen mehrerer Handlungen gleichzeitig gültig sein.

Aktuell kann das Netzwerk nicht zuverlässig unterscheiden, welche Markierungen zur bewerteten Handlung gehören.

Lokale Eligibility-Generationen helfen nur begrenzt:

Die Generation 12 eines Neurons bezeichnet nicht automatisch dieselbe Handlung wie die Generation 12 eines anderen Neurons.

Es fehlt eine gemeinsame, neuronal verfügbare Kontextinformation.

## 1.2 Wichtiger Befund aus B.3

Die Varianten Z0, Z1 und Z2 wurden in B.3 noch nicht als vollständige experimentelle Handlungszuordnungsmechanismen untersucht.

Insbesondere enthält die bisherige Funktion `run_assignment()` lediglich vordefinierte Aussagen zur Zuordnungsfähigkeit.

**B.3.1 muss diese Untersuchung durch echte Implementierungen, Messdaten und kontrollierte Versuche ersetzen.**

Eine statische Ergebnisliste erfüllt den Forschungsauftrag nicht.

---

# 2. Zentrale Forschungsfrage

**Kann EVE durch ihre eigene neuronale Aktivität einen zeitlich begrenzten Handlungskontext aufbauen, der mehrere lokale Feuerentscheidungen miteinander in Beziehung setzt und später eintreffende Rückmeldungen von Rückmeldungen anderer Handlungen unterscheidbar macht?**

Dabei sind vier Eigenschaften getrennt zu untersuchen:

1. **Kontextbildung:** Entsteht während einer Handlung ein lokal verfügbarer neuronaler Zustand?
2. **Kontextverteilung:** Können mehrere beteiligte Neuronen Informationen über denselben Handlungszusammenhang erhalten?
3. **Kontextbindung:** Können lokale Eligibility-Markierungen mit diesem Zustand in Beziehung gesetzt werden?
4. **Rückmeldungszuordnung:** Kann eine spätere Bewertung mit dem passenden neuronalen Kontext zusammentreffen, ohne diesen Zusammenhang technisch vorzugeben?

Die Kernfrage lautet nicht, ob das BIOS eine Handlung identifizieren kann.

Das kann es bereits.

Die Kernfrage lautet, welche Informationen **das neuronale System selbst** zur Unterscheidung verschiedener Erfahrungen besitzen kann.

---

# 3. Verbindliche Forschungsprinzipien

## 3.1 Keine technische Handlungs-ID als neuronale Lösung

Das BIOS darf Handlungen für Forschungszwecke eindeutig nummerieren.

Diese Nummern dürfen jedoch niemals direkt in neuronale Eligibility-Zustände, Rückmeldungssignale oder Kontextspeicher übertragen werden.

Nicht zulässig sind:

- Eine globale Handlungsnummer in jedem beteiligten Neuron.
- Ein vom BIOS generierter eindeutiger Kontextcode je Handlung.
- Eine technische Liste aller an einer Handlung beteiligten Neuronen.
- Das nachträgliche Markieren aller technisch rekonstruierten M3-Kausalitätskandidaten.
- Ein versteckter Tabellenvergleich zwischen technischer Handlungs-ID und lokalen Markierungen.

Auch die bloße Umbenennung einer technischen Handlungs-ID in „neuronales Kontextsignal“ wäre keine gültige Lösung.

## 3.2 Kontext muss neuronal entstehen

Ein Kontext darf entstehen durch:

- Reguläre sensorische Eingangssignale.
- Tatsächlich erfolgte motorische Aktivität.
- Neuronale Eigenrückmeldung.
- Angeborene allgemeine neuronale Verschaltungen.
- Lokale neuronale Aktivität.
- Zeitlich begrenzte lokale Zustandsveränderungen.
- Neuronale Weiterleitung und Wiederaufnahme von Signalen.

Er darf nicht durch Einsicht in den technisch bekannten korrekten Handlungsweg entstehen.

## 3.3 Bestehende Architektur bleibt erhalten

**BIOS:** Ereignisverwaltung, technische I/O, Simulation, Persistenz und unabhängige Forschungsdiagnose.

**Stammhirn:** Angeborene Regeln der Signalverarbeitung, Kontextbildung und lokalen Zustandsänderung.

**Neuronales Netzwerk:** Tatsächliche neuronale Ereignisse, Verbindungen, Membranzustände, G3, Eligibility und mögliche neue Kontextzustände.

Es gibt weiterhin:

- Keinen globalen neuronalen Tick.
- Keine regelmäßigen vollständigen Netzwerkdurchläufe.
- Keine Zeitstempel pro Synapse.
- Keine vorgegebenen semantischen Neuronen.
- Keine neuen rückmeldungsabhängigen Gewichtsänderungen.

---

# 4. Ausgangskonfiguration

Verwende zunächst die in B.3 etablierte Konfiguration.

| Parameter | Wert |
|---|---|
| Neuronen | 256 |
| Reguläre Startsynapsen | 1.024 |
| Geometrie | 8 × 8 × 4 |
| Angeborene Verschaltung | V2 |
| Angeborenes Gewicht | 3 |
| Motorische Schaltung | M3 |
| Motorische Gewichte | 1 und 3 |
| Feuergrenze | 3 |
| Ereignisgedächtnis | G3, 8 Einträge |
| Erholung | R2F |
| Erholungsdauer | 16 |
| Eligibility-Auswahl | L2 |
| Kandidatenkapazität | 2 |
| Entscheidungsplätze | 2 |
| Eligibility-Gültigkeit | 64 |
| Rückmeldungsroute | F2 und F3 |
| Explorationsmodus | E4-neu |
| Lernen | Deaktiviert |
| Wachstum | Deaktiviert |
| Seeds | Mindestens die 64 etablierten |

L1 mit Nullgewicht-Kandidaten bleibt eine Pflichtkontrolle.

Die Konfiguration muss reproduzierbar und variantenweise veränderbar sein.

---

# 5. Forschungsphase I – Analyse der vorhandenen Handlungsereignisse

## 5.1 Ziel

Untersuche zuerst, welche Informationen aus dem bestehenden neuronalen Ablauf grundsätzlich für die Bildung eines Handlungskontextes infrage kommen.

Noch keine neue Kontextarchitektur implementieren.

## 5.2 Zu untersuchende Ereignisse

Dokumentiere die tatsächliche neuronale Reihenfolge von:

1. Sensorischem Eingangsreiz.
2. Aktivität angeborener Verarbeitungspfade.
3. Explorativen Feuerereignissen.
4. Motorischen Datenaktivierungen.
5. M3-Akkumulatoraktivität.
6. Motorischem Abschluss.
7. Sensorischer Eigenrückmeldung.
8. Lokalen Eligibility-Entscheidungen.
9. Erholungsereignissen.
10. Eintreffender Bewertungsrückmeldung.

## 5.3 Diagnostische Fragen

- Welche Aktivität tritt zuverlässig vor einer Handlung auf?
- Welche neuronalen Ereignisse treten nur bei bestimmten Handlungen auf?
- Welche Signale sind über mehrere Neuronen verteilt?
- Welche Verbindungen sind für einen gemeinsamen Handlungskontext erreichbar?
- Welche Signale unterscheiden erfolgreiche von nicht abgeschlossenen Handlungen?
- Wie lange bleiben solche Signale lokal verfügbar?
- Welche Zustände überleben die neuronale Erholung?

## 5.4 Ergebnis

Erstelle eine begründete Übersicht potenzieller Kontextquellen.

Die technische M3-Diagnose dient nur zur nachträglichen Einordnung.

Sie darf keinen neuronalen Kontext erzeugen.

---

# 6. Forschungsphase II – Kontextvarianten

Untersuche mindestens folgende Varianten.

## C0 – Ohne neuronalen Handlungskontext

Unveränderte B.3-Referenz.

Rückmeldungen können auf gültige Eligibility treffen.

Eine eindeutige Zuordnung verschiedener Handlungen ist nicht vorhanden.

Diese Variante bildet die Vergleichsbasis.

## C1 – Neuronale Handlungsgrenzen

Beginn und Ende einer Handlung werden über allgemeine sensorische Neuronenreize kenntlich gemacht.

Dabei wird keine eindeutige Handlungsnummer übertragen.

Die Begrenzungsreize dürfen lediglich ausdrücken:

- Ein neuer Handlungsabschnitt beginnt.
- Ein Handlungsabschnitt endet.

Die Verarbeitung erfolgt über reguläre neuronale Ereignisse.

Zu untersuchen ist, ob solche Signale lokal unterscheidbare Aktivitätsabschnitte erzeugen.

Eine bloße technische Kenntnis von Beginn und Ende ist kein erfolgreicher neuronaler Zuordnungsnachweis.

## C2 – Motorische Kontextbildung durch Eigenaktivität

Hier entsteht Kontext aus tatsächlich ausgeführter motorischer Aktivität.

Verwende die vorhandene sensorische Eigenrückmeldung beziehungsweise eine allgemeine neuronale Kopie motorischer Ausgangsaktivität (*Efference Copy*).

Die neuronalen Signale dürfen nur tatsächlich aufgetretene motorische Aktivität abbilden.

Nicht zulässig ist die Übertragung des gewünschten, aber nicht erzeugten Ergebnisses.

Untersuche:

- Kontextbildung bei vollständiger M3-Handlung.
- Kontextbildung bei unvollständiger Handlung.
- Kontextbildung bei bloßer motorischer Teilaktivität.
- Wiederholte identische Handlungen.
- Unterschiedliche motorische Ausgabemuster.

Die motorische Kontextbildung soll nicht automatisch als eindeutige Identität einer Handlung gelten.

## C3 – Gemeinsame neuronale Kontextaktivität

Untersuche eine kleine, allgemeine neuronale Verschaltung, deren Aktivität mehreren Verarbeitungsbereichen als zeitlich begrenzte Kontextinformation zur Verfügung stehen kann.

Diese Verschaltung darf angeboren sein.

Ihre konkrete Aktivität muss jedoch aus dem tatsächlichen neuronalen Ablauf hervorgehen.

Mögliche Eigenschaften:

- Wenige allgemeine Kontext- oder Relaisneuronen.
- Reproduzierbare, aber nicht handlungsspezifisch vorverdrahtete Verbindungen.
- Lokale Aktivitätszustände.
- Zeitlich begrenzte Kontextspuren.
- Reguläre synaptische Impulse.
- Eigenständiger ereignisgesteuerter Verfall.

Die konkrete technische Ausgestaltung soll aus der Analyse der Phase I abgeleitet und dokumentiert werden.

Es ist nicht vorauszusetzen, dass eine gemeinsame Aktivität automatisch eine eindeutige Handlungsidentität erzeugt.

## C4 – Kombination aus motorischem Kontext und allgemeiner Kontextaktivität

Diese Variante ist optional.

Sie wird nur untersucht, wenn C2 oder C3 einen messbaren Nutzen gegenüber C0/C1 zeigen.

Ziel ist die Frage, ob unterschiedliche tatsächlich entstandene neuronale Informationen gemeinsam eine bessere Zuordnung ermöglichen.

---

# 7. Forschungsphase III – Lokale Kontextspuren

## 7.1 Ziel

Entwickle einen minimalen lokalen neuronalen Zustand, mit dem ein Neuron Informationen über einen zuvor empfangenen Kontextreiz für begrenzte Zeit erhalten kann.

Die Kontextspur ist von Membranpotenzial, Refraktärzustand, G3 und Eligibility zu unterscheiden.

## 7.2 Anforderungen

Eine Kontextspur soll:

- Ausschließlich durch reguläre neuronale Ereignisse entstehen.
- Lokal gespeichert sein.
- Zeitlich begrenzt gültig sein.
- Ohne zusätzliche Eingänge verfallen können.
- Nicht von einem globalen Netzwerkdurchlauf abhängen.
- Keinen synaptischen Zeitstempel benötigen.
- Persistierbar sein.

## 7.3 Experimentelle Varianten

Untersuche mindestens:

**P0 – Keine Kontextspur**

Referenz ohne zusätzliche Speicherung.

**P1 – Einzelner lokaler Kontextzustand**

Das Neuron hält eine begrenzte Information über die jüngste Kontextaktivität.

**P2 – Mehrere begrenzte Kontextspuren**

Das Neuron kann Informationen mehrerer zeitlich naher Kontextaktivierungen unterscheiden.

Die Zahl gleichzeitiger Spuren bleibt klein und konfigurierbar.

## 7.4 Entscheidender Unterschied

Ein lokaler Kontextzustand ist nicht automatisch eine Handlungs-ID.

Er muss durch tatsächlich empfangene neuronale Aktivität erklärbar sein.

Wenn zwei verschiedene Handlungen denselben Kontextzustand erzeugen, sind sie anhand dieses Zustands nicht eindeutig unterscheidbar.

Diese Mehrdeutigkeit muss gemessen und dokumentiert werden.

---

# 8. Forschungsphase IV – Verbindung zwischen Kontext und Eligibility

## 8.1 Ausgangssituation

Eine Eligibility-Markierung entsteht bei einer lokalen neuronalen Feuerentscheidung.

Ein neuronaler Handlungskontext kann vor, während oder nach dieser Feuerentscheidung aktiv sein.

## 8.2 Forschungsfrage

**Kann eine lokale Feuerentscheidung mit einer tatsächlich vorhandenen neuronalen Kontextspur verbunden werden, ohne eine globale Handlungs-ID zu verwenden?**

## 8.3 Versuchsvarianten

### B0 – Eligibility ohne Kontextbindung

Unveränderte B.2/B.3-Referenz.

### B1 – Bindung bei der Feuerentscheidung

Wenn bei einer Feuerentscheidung eine gültige lokale Kontextspur vorhanden ist, wird deren lokal verfügbarer Zustand mit der Eligibility-Entscheidung in Beziehung gesetzt.

Die Bindung darf nur bereits lokal vorhandene Informationen verwenden.

### B2 – Bindung durch zeitliche Überlappung

Untersuche, ob eine Eligibility-Markierung und eine später eintreffende Kontextaktivität lokal miteinander in Beziehung gesetzt werden können.

Dabei darf die Markierung nicht nachträglich auf Basis technischer Beteiligtenlisten korrigiert werden.

Die Regel für ein gültiges Zusammentreffen muss vor dem Versuch festgelegt werden.

## 8.4 Wichtige Einschränkung

Die Bindung darf nicht einfach lauten:

„Alle Markierungen seit technischem Handlungsbeginn gehören zur aktuellen technischen Handlungsnummer.“

Dies wäre eine verdeckte globale Handlungszuordnung.

Die Verbindung muss sich aus lokal nachweisbaren neuronalen Ereignissen ergeben.

## 8.5 Messgrößen

Erfasse:

- Feuerentscheidungen mit verfügbarer Kontextspur.
- Feuerentscheidungen ohne Kontextspur.
- Gültige lokale Kontext-Eligibility-Verbindungen.
- Mehrdeutige Verbindungen.
- Verlorene Kontextinformationen.
- Überschriebene Markierungen.
- Fehlende oder abgelaufene Spuren.

---

# 9. Forschungsphase V – Rückmeldungszuordnung

## 9.1 Ziel

Untersuche, ob eine neuronal eintreffende Bewertung mit einer passenden lokalen Kontext-Eligibility-Verbindung zusammentreffen kann.

## 9.2 Trennung der Informationen

Die technische Forschungsdiagnose kennt:

- Die tatsächliche Handlung.
- Den Zeitpunkt der Handlung.
- Die technisch zugehörige Bewertung.
- Den beobachteten M3-Handlungsweg.

Das neuronale System kennt ausschließlich:

- Seine tatsächlich empfangenen Reize.
- Seine eigenen Aktivitäten.
- Seine lokalen Zustände.
- Seine gültigen Eligibility-Markierungen.
- Seine lokalen Kontextspuren.
- Die neuronal eintreffende Bewertung.

Diese Trennung muss technisch überprüfbar sein.

## 9.3 Lokale Zuordnungsentscheidung

Die neuronale Verarbeitung darf eine rein diagnostisch beobachtbare lokale Zuordnung hervorbringen.

Zu unterscheiden sind:

- Keine gültige Eligibility.
- Eligibility vorhanden, aber kein passender Kontext.
- Ein möglicher lokaler Kontexttreffer.
- Mehrere mögliche Kontexttreffer.
- Bereits abgelaufener Kontext.
- Durch Rückmeldung selbst entstandener Kontext.

Ein mehrdeutiger Treffer darf nicht als eindeutige Zuordnung gezählt werden.

## 9.4 Keine nachträgliche Reparatur

Wenn die passende lokale Kontextinformation fehlt, gilt die Zuordnung als nicht gelungen.

Sie darf nicht mithilfe der technischen Handlungs-ID ergänzt werden.

---

# 10. Forschungsphase VI – Identische und unterschiedliche Handlungen

## 10.1 Ziel

Untersuche, welche Unterschiede im neuronalen Ablauf tatsächlich ausreichen, um Handlungen auseinanderzuhalten.

## 10.2 Versuchsfolgen

Verwende mindestens:

- `AB → AB`
- `AB → BA`
- `AB → AA`
- `AA → AA`
- `BB → BB`
- Alternierende `AB`-/`BA`-Eingaben.

Die Folgen müssen über reproduzierbare Seeds untersucht werden.

## 10.3 Besonderer Prüfpunkt

Zwei identische Handlungen dürfen nicht allein deshalb als eindeutig unterscheidbar gelten, weil die technische Simulation sie als Handlung 1 und Handlung 2 kennt.

Wenn der neuronale Zustand keine unterscheidbaren Informationen enthält, ist die Zuordnung mehrdeutig.

Das ist ein zulässiger Forschungsbefund.

## 10.4 Zu untersuchen

- Unterschiedliche Handlungen mit gleichem Kontext.
- Identische Handlungen mit unterschiedlichem Kontext.
- Unterschiedliche motorische Aktivität bei gleicher Eingabe.
- Gleiche motorische Aktivität bei unterschiedlicher Eingabe.
- Erfolgreiche und unvollständige Handlungen.
- Explorationsbedingte Aktivität ohne gültigen Abschluss.

---

# 11. Forschungsphase VII – Verzögerte und vertauschte Rückmeldungen

## 11.1 Versuchsvarianten

### R0 – Unmittelbare Rückmeldung

Die Bewertung trifft direkt nach der Handlung ein.

### R1 – Verzögerte Rückmeldung

Die Bewertung trifft nach einem definierten Zeitabstand ein.

### R2 – Überlappende Handlungen

Eine zweite Handlung beginnt, bevor die Bewertung der ersten Handlung angekommen ist.

### R3 – Vertauschte Reihenfolge

Die Bewertung zu Handlung B trifft vor der Bewertung zu Handlung A ein.

### R4 – Fehlende Rückmeldung

Eine Handlung erhält keine Bewertung.

### R5 – Gleiche Bewertungen

Zwei Handlungen erhalten identische Erfolgswerte.

### R6 – Unterschiedliche Bewertungen

Zwei Handlungen erhalten unterschiedliche Erfolgswerte.

## 11.2 Kritische Anforderungen

Die neuronale Zuordnung darf nicht davon ausgehen:

- Dass Bewertungen grundsätzlich in Handlungsreihenfolge eintreffen.
- Dass nur eine Handlung gleichzeitig relevant sein kann.
- Dass identische Bewertungswerte dieselbe Handlung betreffen.
- Dass jede Handlung überhaupt eine Bewertung erhält.
- Dass die zuletzt vorhandene Eligibility automatisch die richtige ist.

Wenn die neuronale Architektur diese Fälle nicht unterscheiden kann, ist die Zuordnung als mehrdeutig zu dokumentieren.

## 11.3 Zeitliche Parameter

Vergleiche Rückmeldungen:

- Sofort.
- Vor Ablauf der Kontextspur.
- Genau beim Ablauf.
- Nach Ablauf.
- Nach einer weiteren Handlung.
- Nach mehreren weiteren Handlungen.

Die Ordnung bei zeitgleichen Ereignissen ist deterministisch festzulegen.

---

# 12. Forschungsphase VIII – Rückmeldungsinduzierte Störaktivität

B.3 zeigte, dass Rückmeldungen selbst zahlreiche neue Eligibility-Markierungen erzeugen.

Dieses Problem besteht weiterhin.

## 12.1 Forschungsziel

Untersuche, ob neuronale Kontextspuren durch Rückmeldungsaktivität beeinflusst, überschrieben oder neu erzeugt werden.

## 12.2 Herkunftsunterscheidung

Die bereits vorhandene technische Herkunftsdiagnose `feedback_origin` bleibt als Kontrollinstrument bestehen.

Eine zusätzliche Kontextinformation darf nicht rückwirkend den Eindruck erzeugen, sie habe bereits während der zuvor bewerteten Handlung existiert.

## 12.3 Wichtige Kontrollen

Untersuche:

- Rückmeldung ohne vorherige Handlung.
- Rückmeldung nach Ablauf aller Handlungskontexte.
- Rückmeldung bei mehreren gleichzeitig gültigen Kontextspuren.
- Rückmeldung, die selbst neue Kontexte erzeugt.
- Rückmeldung, die ältere Kontexte überschreibt.
- Mehrere unmittelbar aufeinanderfolgende Rückmeldungen.
- Rückmeldungen, die motorische Aktivität auslösen.

## 12.4 Messgrößen

- Neu durch Rückmeldung entstandene Kontextspuren.
- Überschriebene vorherige Kontextspuren.
- Neu erzeugte Eligibility-Markierungen.
- Fälschlich als vorausgehend gezählte Markierungen.
- Fehlzuordnungen aufgrund von Rückmeldungskaskaden.

**Rückmeldungsinduzierte Aktivität darf nicht als nachträglicher Beweis einer früheren Handlungsbeteiligung verwendet werden.**

---

# 13. Forschungsphase IX – Fortlaufender Betrieb

## 13.1 Ziel

Untersuche die Kontextarchitektur bei wiederholter neuronaler Aktivität ohne Wiederherstellung des Ausgangszustands.

## 13.2 Versuchsgrößen

Mindestens:

- 2 Antwortphasen.
- 8 Antwortphasen.
- 32 Antwortphasen.
- 128 Antwortphasen.

Verwende dieselben Seeds und Aufgabenfolgen für alle Vergleichsvarianten.

## 13.3 Kontrollen

Vergleiche:

- Fortgeführten Zustand mit Kontext.
- Fortgeführten Zustand ohne Kontext.
- Wiederhergestellte Referenzzustände zur Einordnung.
- R1 und R2F in ausgewählten Vergleichsläufen.

Die wiederhergestellten Zustände dienen ausschließlich als Kontrollgruppe.

Die eigentliche neuronale Handlungsfähigkeit muss ohne Zustandswiederherstellung funktionieren.

## 13.4 Stabilitätsfragen

- Bleiben motorische Handlungen möglich?
- Verändern Kontextsignale die bekannten M3-Erfolgsraten?
- Entstehen zusätzliche unkontrollierte Feuerkaskaden?
- Werden Kontextspuren regelmäßig freigegeben?
- Können alte Markierungen neue Erfahrungen überlagern?
- Bleibt die Verarbeitung bei langen Folgen reproduzierbar?

---

# 14. Kontrollmatrix

Mindestens folgende Kontrollen sind verbindlich:

| Kontrolle | Untersuchung |
|---|---|
| K0 | B.3 ohne Handlungskontext |
| K1 | Kontextreize vorhanden, Kontextbindung deaktiviert |
| K2 | Kontextbindung aktiv, Rückmeldung deaktiviert |
| K3 | Rückmeldung ohne vorherige Handlung |
| K4 | Rückmeldung nach Ablauf der Markierung |
| K5 | Falsche oder vertauschte Rückmeldungsreihenfolge |
| K6 | Identische aufeinanderfolgende Handlungen |
| K7 | Unterschiedliche Handlungen mit gleicher Bewertung |
| K8 | Unterbrochene M3-Handlung |
| K9 | Technischer Abschluss ohne nachgewiesenen M3-Weg |
| K10 | L1 gegen L2 |
| K11 | Rückmeldungsinduzierte Eligibility |
| K12 | C2/C3 mit deaktivierter Kontextweiterleitung |
| K13 | Kontextinformation absichtlich gestört oder verzögert |
| K14 | Fortgeführtes R1 gegen R2F |
| K15 | Vollständig deaktivierte neue Mechanismen |

Alle Kontrollen müssen mit tatsächlichen Versuchsläufen und dokumentierten Rohdaten untersucht werden.

---

# 15. Messung der Zuordnungsqualität

Dies ist der wichtigste Teil der Forschungsstufe.

Eine hohe Zahl erreichter Eligibility-Markierungen allein genügt nicht.

## 15.1 Bewertungsgrößen

Unterscheide mindestens:

**Kontextabdeckung**

Wie viele tatsächlich beteiligte neuronale Feuerentscheidungen verfügten über einen gültigen lokalen Kontext?

**Kontextselektivität**

Wie viele mit einem Kontext verbundene Feuerentscheidungen gehörten tatsächlich zur untersuchten Handlung?

**Rückmeldungserreichbarkeit**

Wie viele gültige Kontext-Eligibility-Zustände wurden über einen echten neuronalen Rückmeldungsweg erreicht?

**Zuordnungsgenauigkeit**

Wie oft wurde eine eintreffende Bewertung der richtigen Handlung zugeordnet?

**Fehlzuordnungsrate**

Wie oft wurde eine Rückmeldung einer falschen Handlung zugeordnet?

**Mehrdeutigkeitsrate**

Wie oft standen mehrere mögliche Handlungen beziehungsweise Kontexte zur Auswahl?

**Nichtzuordnungsrate**

Wie häufig war keine ausreichend belastbare Zuordnung möglich?

## 15.2 Unabhängige Wahrheitsgrundlage

Die Forschungsdiagnose darf anhand ihrer technischen Ereignisprotokolle prüfen, welche neuronalen Entscheidungen tatsächlich während welcher Handlung entstanden.

Diese Informationen dürfen ausschließlich zur Bewertung der Versuchsergebnisse verwendet werden.

Sie dürfen nicht in die neuronale Zuordnungsentscheidung einfließen.

## 15.3 Keine künstliche Verbesserung der Statistik

Mehrere Rückmeldungsimpulse an dasselbe Neuron dürfen nicht automatisch als mehrere unabhängig korrekt zugeordnete Handlungen zählen.

Berichte deshalb getrennt:

- Einzelne Rückmeldungsimpulse.
- Einzigartige erreichte Neuronen.
- Einzigartige erreichte Eligibility-Entscheidungen.
- Einzigartige zuordenbare Handlungen.
- Tatsächlich eindeutige Zuordnungen.

Die Auswertung muss Fehlzuordnungen und Mehrdeutigkeiten ausdrücklich sichtbar machen.

## 15.4 Erfolgskriterium

Eine neue Kontextvariante gilt nur dann als nachweislich besser, wenn sie unter identischen Bedingungen die tatsächliche Handlungszuordnung gegenüber C0 verbessert.

Insbesondere darf ein Anstieg der bloßen Rückmeldungserreichbarkeit nicht als verbesserte Handlungszuordnung ausgegeben werden.

Die Zielwerte und Auswertungsregeln sind vor den abschließenden Vergleichsläufen festzulegen.

Eine unzureichende oder nicht eindeutig nachweisbare Verbesserung ist als negatives Ergebnis zu dokumentieren.

---

# 16. Ressourcenverbrauch

Die gesamte Kontextarchitektur muss zum ressourcensparenden EVE-Konzept passen.

## 16.1 Technische Anforderungen

- Ereignisgesteuerte Verarbeitung.
- Keine globalen neuronalen Aktualisierungen.
- Kompakter lokaler Zustand.
- Keine Zeitstempel pro Synapse.
- Begrenzte Zahl paralleler Kontextspuren.
- Kein unbegrenztes Sammeln alter Handlungsinformationen.
- Keine versteckte technische Handlungsdatenbank im neuronalen Kern.
- Keine Speicherallokation für jedes einzelne synaptische Ereignis, sofern vermeidbar.

## 16.2 Messgrößen

Erfasse:

- Zusätzliche Neuronen für angeborene Kontextwege.
- Zusätzliche Synapsen.
- Zusätzliche Byte pro Neuron.
- Zusätzliche Byte je aktivem Kontextzustand.
- Zusätzliche geplante Ereignisse.
- Durchschnittlich gleichzeitig aktive Kontextspuren.
- Maximale Anzahl aktiver Kontextspuren.
- Laufzeitkosten je Kontextaktivierung.
- Kosten des Kontextverfalls.
- Synaptische Ereignisse pro Sekunde.
- Maximale Warteschlangengröße.

## 16.3 Skalierung

Vermesse 256 Neuronen tatsächlich.

Erstelle zusätzlich Schätzungen für:

- 10.000 Neuronen.
- 100.000 Neuronen.
- 1.000.000 Neuronen.

Gemessene und extrapolierte Werte sind eindeutig zu unterscheiden.

Prüfe außerdem, ob neue Kontextzustände proportional zur Neuronenzahl wachsen oder abhängig von der Zahl tatsächlich aktiver Handlungen gehalten werden können.

---

# 17. Persistenz

Eine gültige neuronale Kontextspur ist Teil des neuronalen Zustands.

Die Persistenz muss deshalb unterstützen:

1. Speichern während einer Handlung.
2. Speichern mit aktiven Kontextspuren.
3. Speichern mit mehreren gleichzeitigen Kontextspuren.
4. Speichern mit aktiven Eligibility-Markierungen.
5. Speichern bei ausstehenden Rückmeldungen.
6. Speichern bei ausstehenden Kontextabläufen.
7. Wiederherstellung mit identischer Ereignisreihenfolge.
8. Wirkungsloses Verwerfen veralteter Ablaufereignisse.
9. Abwärtskompatibilität bisheriger Zustandsformate.

Nach Speichern und Laden muss eine identische Fortsetzung denselben neuronalen Zustand erzeugen.

Technische Handlungs-IDs dürfen weiterhin nicht als neuronale Kontextinformationen wiederhergestellt werden.

---

# 18. Automatisierte Tests

Ergänze die bestehende Testsuite um mindestens folgende Prüfungen:

1. Kein Kontext bei deaktiviertem Mechanismus.
2. Kontextentstehung nur durch tatsächlich verarbeitete neuronale Ereignisse.
3. Keine technische Handlungs-ID in neuronalen Kontextzuständen.
4. C1 erzeugt reguläre neuronale Grenzreize.
5. C2 nutzt ausschließlich tatsächlich aufgetretene motorische Aktivität.
6. C3 verwendet allgemeine, handlungsunabhängige Verschaltung.
7. Kontextspur entsteht gemäß ihrer lokalen Regel.
8. Kontextspur besitzt begrenzte Gültigkeit.
9. Kontextspur verfällt ohne weitere Eingänge.
10. Kontextablauf verändert keine Synapsengewichte.
11. Kontextablauf löst kein eigenständiges Feuern aus.
12. G3 bleibt unabhängig von Kontextzuständen.
13. R2F bleibt unabhängig von Kontextzuständen.
14. Eligibility bleibt separat konfigurierbar.
15. Kontextbindung verwendet nur lokal vorhandene Informationen.
16. Fehlende lokale Kontextinformation erzeugt keine künstliche Bindung.
17. Identische Handlungen werden nicht durch technische IDs unterscheidbar gemacht.
18. Unterschiedliche Handlungen mit gleichem neuronalen Kontext bleiben als mehrdeutig erkennbar.
19. Verzögerte Rückmeldungen werden korrekt verarbeitet.
20. Vertauschte Rückmeldungen werden nicht anhand ihrer Ankunftsreihenfolge falsch als korrekt eingestuft.
21. Rückmeldung ohne Handlung erzeugt keine gültige vorherige Handlungszuordnung.
22. Abgelaufene Kontextspuren werden nicht reaktiviert.
23. Rückmeldungsinduzierte Markierungen werden getrennt erfasst.
24. Unterbrochene M3-Pfade bleiben als Kontrollfälle erhalten.
25. Unvollständige Handlungen werden nicht automatisch zu erfolgreichen Handlungen erklärt.
26. Mehrere gleichzeitig gültige Kontextspuren werden berücksichtigt.
27. Lokale Zustände können bei Bedarf deterministisch überschrieben werden.
28. Persistenz aktiver Kontextspuren.
29. Persistenz ausstehender Ablaufereignisse.
30. Fortsetzung nach Wiederherstellung ist deterministisch.
31. Keine neue rückmeldungsabhängige Gewichtsänderung.
32. Die bestehenden 141 B.3-Tests bleiben bestehen und erfolgreich.

Die Zuordnungsvarianten müssen echte dynamische Tests besitzen.

Vordefinierte CSV-Ergebniszeilen ohne zugehörige Versuchsdurchführung sind nicht zulässig.

---

# 19. Forschungsbericht

Erstelle einen vollständigen Forschungsbericht mit folgenden Abschnitten.

## A. Ausgangssituation

Bekannte Probleme aus B.2 und B.3.

## B. Neuronale Kontextquellen

Welche tatsächlich vorhandenen Signale eignen sich als Ausgangspunkt?

## C. Technische Umsetzung

Kontextwege, lokale Zustände und Ereignisverarbeitung.

## D. Vergleich C0 bis C4

Entstehung, Verteilung und Erhalt neuronaler Kontexte.

## E. Verbindung mit Eligibility

Qualität der lokalen Kontextbindung.

## F. Rückmeldungszuordnung

Korrekte, falsche, fehlende und mehrdeutige Zuordnungen.

## G. Überlappende Handlungen

Verzögerung, vertauschte Bewertungen und identische Handlungen.

## H. Rückmeldungsinduzierte Störungen

Neue Markierungen, Kontextveränderungen und mögliche Fehlzuordnungen.

## I. Langzeitbetrieb

Mindestens bis 128 fortgeführte Handlungen.

## J. Ressourcenverbrauch

Laufzeit, Speicher und Skalierung.

## K. Grenzen des Mechanismus

Welche Fälle sind mit der untersuchten neuronalen Architektur grundsätzlich noch nicht unterscheidbar?

## L. Schlussfolgerung

Welche Architektur bietet die beste experimentelle Grundlage für eine spätere rückmeldungsabhängige Veränderung synaptischer Gewichte?

---

# 20. Lieferumfang

Erstelle:

1. Vollständigen Forschungsbericht B.3.1.
2. Technische Spezifikation neuronaler Handlungskontexte.
3. Implementierungen der tatsächlich untersuchten Kontextvarianten.
4. Konfigurierbare lokale Kontextspuren.
5. Experimentelle Verbindung mit Eligibility.
6. Reproduzierbare Versuchsskripte.
7. Maschinenlesbare Rohdaten.
8. Zuordnungsanalyse über mindestens 64 Seeds.
9. Versuche mit identischen und unterschiedlichen Handlungen.
10. Versuche mit verzögerten und vertauschten Rückmeldungen.
11. Negativkontrollen.
12. Performance- und Speicherdaten.
13. Persistenztests.
14. Vollständige automatisierte Regressionstests.
15. Eine Entscheidungsvorlage für den möglichen Übergang zu B.4.

Vorhandene Dokumentation, Forschungsdaten und bisherige Projektfunktionen müssen erhalten bleiben.

Es darf keine technisch vorgegebene Handlungszuordnung in den neuronalen Kern eingebaut werden.

---

# 21. Verbindliche Forschungsgrenzen

Noch nicht implementieren:

- Positive rückmeldungsabhängige Gewichtsänderungen.
- Negative rückmeldungsabhängige Gewichtsänderungen.
- Automatische synaptische Löschung.
- Hemmende Synapsen.
- STDP.
- Vorgegebene Bedeutung einzelner neuronaler Kontexte.
- Semantische Wortrepräsentationen.
- Eigenständige neuronale Erfolgserwartung.
- Vorhersagefehler als Lernregel.
- Eine allgemeine globale Handlungsnummer im neuronalen Zustand.
- Eine technische Rückwärtsübertragung des M3-Handlungsweges.
- Produktive Chatto-Kommunikation.

B.3.1 dient der Untersuchung neuronaler Handlungszuordnung.

Die synaptischen Gewichte bleiben durch neue Bewertungsmechanismen unverändert.

---

# 22. Abschlussfragen

Am Ende müssen folgende Fragen empirisch beantwortet sein:

1. Kann EVE einen Handlungskontext aus eigener neuronaler Aktivität bilden?
2. Können mehrere beteiligte Neuronen über diesen Kontext Informationen teilen?
3. Kann eine lokale Eligibility-Markierung mit einem vorhandenen Kontext in Beziehung gesetzt werden?
4. Kann eine Rückmeldung einen solchen Kontext neuronal erreichen?
5. Können unterschiedliche Handlungen zuverlässig unterschieden werden?
6. Können identische, aufeinanderfolgende Handlungen unterschieden werden?
7. Wie verlässlich ist die Zuordnung bei verzögerten Rückmeldungen?
8. Wie häufig werden Bewertungen der falschen Handlung zugeordnet?
9. Wie werden mehrere gültige Handlungskontexte behandelt?
10. Was geschieht, wenn eine passende Kontextspur bereits abgelaufen ist?
11. Wie stark stört die neuronale Rückmeldung selbst den Kontextzustand?
12. Welche zusätzlichen Ressourcen benötigt der Mechanismus?
13. Welche Unterscheidungen bleiben ohne zusätzliche neuronale Informationen unmöglich?
14. Ist die erzielte Zuordnungsqualität ausreichend, um einen begrenzten ersten synaptischen Lernversuch zu rechtfertigen?

Falls die Zuordnung nicht zuverlässig gelingt, ist dies ausdrücklich als Forschungsergebnis festzuhalten.

Eine fehlende Fähigkeit darf nicht durch eine verborgene technische Handlungs-ID ersetzt werden.

---

# 23. Entscheidung über Forschungsstufe B.4

Erst nach Abschluss dieser Untersuchung wird gemeinsam entschieden, ob EVE eine erste rückmeldungsabhängige synaptische Lernregel erhalten soll.

Dafür müssen mindestens folgende Voraussetzungen untersucht und bewertet sein:

- Die Rückmeldung erreicht neuronale Eligibility-Zustände.
- Der passende Handlungskontext ist neuronal verfügbar.
- Fehlzuordnungen sind messbar und hinreichend begrenzt.
- Rückmeldungsinduzierte Markierungen werden nicht rückwirkend einer Handlung zugerechnet.
- Nullgewicht-Synapsen bleiben grundsätzlich lernfähig.
- Die neuronale Aktivität bleibt stabil.
- Der Ressourcenverbrauch bleibt beherrschbar.

Erst danach darf die Frage gestellt werden, wie eine positive oder negative Rückmeldung eine lokal markierte synaptische Verbindung verändern könnte.

---

## Zentraler Forschungsgrundsatz

**Eine Erfahrung besteht nicht nur aus einer Handlung und deren Bewertung. Entscheidend ist, dass das neuronale System beide Ereignisse miteinander in Beziehung setzen kann.**

Das Stammhirn darf eine Handlung beobachten und bewerten.

Das BIOS darf diese Vorgänge technisch dokumentieren.

Aber das neuronale Netzwerk muss den Zusammenhang aus seinen eigenen Signalen und Zuständen herstellen können.

**Ziel von B.3.1 ist daher nicht, EVE mitzuteilen, welche Handlung bewertet wurde, sondern herauszufinden, ob EVE einen solchen Zusammenhang neuronal abbilden kann.**

Nur dann können wir später sinnvoll untersuchen, ob sie aus ihren eigenen Erfahrungen lernt.
