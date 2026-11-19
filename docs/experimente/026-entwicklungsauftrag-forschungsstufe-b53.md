# Entwicklungsauftrag 026 – Forschungsstufe B.5.3

## Lokale Ursachen transienter Abrufstörungen

**Projekt:** EVE – Emergent Virtual Evolution
**Technologie:** C++17
**Ausgangsbasis:** `codex/research-stage-b52-memory-stability`
**Forschungsbranch:** `codex/research-stage-b53-transient-recall-causes`
**Forschungsstatus:** Kausale Grundlagenforschung ohne vorab freigegebene Architekturänderung

---

## 1. Ausgangslage

EVE besitzt inzwischen:

- Ereignisgesteuerte Neuronen mit binären Feuerereignissen.
- Gewichtete erregende und hemmende Synapsen.
- Lokales G3-Ereignisgedächtnis.
- Zeitlich begrenzte Eligibility-Spuren.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Lokale inhibitorische Konkurrenz.
- Persistente neuronale Zustände.
- Experimentell nachgewiesenen Erwerb und Wiedererwerb autonomer Reiz-Handlungs-Beziehungen.

Die bisherige Forschung hat gezeigt, dass EVE neuronale Fähigkeiten erwerben kann, diese jedoch unter weiteren Erfahrungen häufig nicht zuverlässig verfügbar bleiben.

B.5.2 untersuchte deshalb die Trennung zwischen synaptischer Speicherung und momentaner neuronaler Abrufbarkeit.

### Ergebnisse aus B.5.2

Untersucht wurden:

- 768 neuronale Netze.
- 393.216 natürliche Trainingsepisoden.
- 66.159 beobachtete Fähigkeitsverluste.
- 15.194 detaillierte Verlustzustände.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnungen.
- Unabhängige Entdeckungs- und Bestätigungs-Seeds.

### Kausale Befunde

| Untersuchung | Ergebnis |
|---|---:|
| Detailliert untersuchte Verluste | 15.194 |
| Wiederherstellung mit früherem transientem Gesamtzustand | 14.731 |
| Verluste ohne Gewichtsänderung | 12.452 |
| Verluste, die trotz früherem transientem Zustand bestehen bleiben | 463 |
| Verluste mit nachgewiesener Faktorinteraktion | 296 |

Die Wiederherstellung von 14.731 Antworten erfolgte mit unveränderten neuen synaptischen Gewichten.

Damit wurde experimentell gezeigt, dass die momentane Abrufbarkeit nicht allein aus den gespeicherten Gewichten folgt.

Allerdings umfasst der verwendete transiente Faktor mehrere Zustandskomponenten gleichzeitig.

### Methodische Einschränkung

Die 15.194 detaillierten Verluste sind keine repräsentative Zufallsstichprobe sämtlicher 66.159 Verlustereignisse.

Insbesondere sind alle 12.452 Verluste ohne Gewichtsänderung enthalten.

Unter den übrigen 2.742 Fällen mit Gewichtsänderung lassen sich 2.279 durch den früheren transienten Gesamtzustand wiederherstellen.

Die Ergebnisse müssen daher getrennt nach Auswahlgruppe ausgewertet werden.

---

## 2. Zentrale Forschungsfrage

**Welche einzelnen lokalen neuronalen Zustandskomponenten oder Kombinationen dieser Komponenten sind kausal dafür verantwortlich, dass eine bereits erworbene Reiz-Handlungs-Beziehung vorübergehend nicht autonom abgerufen werden kann?**

Die bisherige T-Gesamtkomponente soll in ihre funktionalen Bestandteile zerlegt werden.

Dabei ist zu unterscheiden zwischen:

1. Zuständen, die neuronales Feuern unmittelbar beeinflussen.
2. Zuständen, die lediglich zukünftige Plastizität beeinflussen.
3. Technischen Zählern ohne unmittelbare Verhaltenswirkung.
4. Zuständen, deren Wirkung erst durch Kombination mit anderen Zuständen entsteht.

Das Ziel ist kein automatischer Reset und keine technische Herstellung einer richtigen Antwort.

Es geht um die Identifikation der tatsächlichen lokalen Ursachen.

---

## 3. Forschungshypothesen

### H1 – Membranpotenziale

Die vorhandenen Membranpotenziale beeinflussen, ob nach einer sensorischen Reizpräsentation die richtige neuronale Feuergrenze erreicht wird.

Unterschiedliche Membranpotenziale können bei identischen Synapsengewichten zu unterschiedlichen autonomen Entscheidungen führen.

### H2 – Lokales Ereignisgedächtnis G3

Die lokale Ereignisgeschichte beeinflusst möglicherweise nicht nur die Lernberechtigung, sondern unmittelbar oder mittelbar auch die nachfolgende neuronale Aktivität.

Die tatsächlichen Lese- und Schreibwirkungen des G3-Zustands sind getrennt zu untersuchen.

### H3 – Eingangs- und Aktivitätszustände

Neuronale Eingangsereigniszähler und synaptische Aktivitätszustände könnten die weitere Verarbeitung beeinflussen.

Es ist ausdrücklich zu prüfen, ob diese Werte überhaupt verhaltenswirksam sind oder lediglich technische beziehungsweise diagnostische Informationen darstellen.

### H4 – Eligibility-Spuren

Vorhandene Eligibility-Spuren könnten über nachfolgende Modulation die Entwicklung eines Abrufs beeinflussen.

Ob sie auch ohne weitere Modulation einen unmittelbaren E1-Abruf beeinflussen, ist offen.

### H5 – Zeitliche Ereignisfolge

Die Reihenfolge und der zeitliche Abstand neuronaler Eingänge können den Zustand der Empfängerneuronen verändern.

Ein wiederholter Reiz kann daher bei unveränderten Gewichten unterschiedlich beantwortet werden.

### H6 – Kombinierte transiente Zustände

Einzelne Zustandskomponenten können für sich unwirksam sein, während ihre Kombination eine andere autonome Antwort erzeugt.

### H7 – Diagnosebedingte Zustandsveränderung

Die diagnostische Präsentation eines Reizes verändert neuronale Zustände und kann die Antwort auf einen nachfolgenden Reiz beeinflussen.

Diese Wirkung ist von natürlicher Interferenz durch andere Erfahrungen zu trennen.

Alle Hypothesen sind unabhängig und gegebenenfalls in Kombination zu prüfen.

---

## 4. Phase A – Referenz und technische Absicherung

### A1 – Ausgangsbasis

Beginne auf dem vorhandenen Forschungsstand B.5.2.

Prüfe:

- Branch-Historie.
- Unveränderte neuronale Kernimplementierung.
- Vollständigkeit der Forschungsdaten.
- Vorhandensein sämtlicher bisherigen Regressionstests.
- Bestehende Persistenzformate.

Die bisherigen 525 Kerntests müssen weiterhin bestehen.

### A2 – Referenzreproduktion

Reproduziere die zentralen B.5.2-Ergebnisse:

- 15.194 detaillierte Verlustzustände.
- 12.452 Verluste ohne Gewichtsänderung.
- 14.731 Wiederherstellungen durch den früheren transienten Gesamtzustand.
- 463 nicht wiederhergestellte Zustände.
- Die vollständige S×I×T-Faktormatrix.
- E1 und E2.
- Normale und inverse Lernbedingungen.
- Unabhängige Bestätigungs-Seeds.

Die ursprünglichen Daten dürfen nicht überschrieben werden.

### A3 – Keine verdeckte Änderung

Die neuronale Architektur bleibt unverändert:

- 32 Neuronen.
- Feuergrenze 2.
- Gewichtsgrenze 3.
- G3.
- `active_phase`-Eligibility.
- Ternäre Modulation.
- Geschützte Exploration.
- Lernbare Hemmung.

Synapsenwachstum bleibt deaktiviert.

---

## 5. Phase B – Inventur des transienten Zustands

Vor einer neuen Kausaluntersuchung muss vollständig geklärt werden, welche Runtime-Informationen tatsächlich verhaltensrelevant sein können.

### B1 – Zustandskomponenten

Untersuche mindestens:

| Kennung | Zustandskomponente |
|---|---|
| T1 | Neuronales Membranpotenzial |
| T2 | Eingangsereigniszähler |
| T3 | Lokales G3-Ereignisgedächtnis |
| T4 | Eligibility-Spuren und ihre Generationen |
| T5 | Synaptische Aktivitätszähler |
| T6 | Refraktär- und Erholungszustände |
| T7 | Simulierte Zeit und zeitabhängige Ereignisse |
| T8 | Ausstehende Ereignisse und Warteschlangenzustand |
| T9 | Sonstige persistierte verhaltenswirksame Zustände |

Die Tabelle ist eine Prüfliste, keine Aussage darüber, dass jede Komponente eine neuronale Entscheidung beeinflusst.

### B2 – Tatsächliche Verwendung im Code

Untersuche für jede Komponente:

- Wo wird sie angelegt?
- Durch welche Ereignisse wird sie verändert?
- Welche Funktionen lesen sie?
- Beeinflusst sie unmittelbar neuronales Feuern?
- Beeinflusst sie ausschließlich Plastizität?
- Dient sie lediglich Diagnostik oder Statistik?
- Wird sie persistiert?
- Wird sie bei Zustandskopien vollständig übernommen?

Erstelle daraus eine technische Wirkungsübersicht.

### B3 – Nichtfunktionale Zustände

Komponenten, die ausschließlich statistische Informationen darstellen und von keiner verhaltenswirksamen Berechnung gelesen werden, sind als solche zu kennzeichnen.

Sie dürfen nicht allein aufgrund unterschiedlicher Werte als Ursache einer Abrufstörung bezeichnet werden.

Ihre Wirkungslosigkeit ist zusätzlich durch geeignete Kontrollversuche abzusichern.

---

## 6. Phase C – Kontrollierte Zustandskopien

### C1 – Auswahl realer Verlustzustände

Verwende dieselben vorab definierten Auswahlregeln wie B.5.2.

Untersuche getrennt:

- Verluste ohne synaptische Gewichtsänderung.
- Verluste mit sensorischen Änderungen.
- Verluste mit inhibitorischen Änderungen.
- Verluste mit kombinierten Gewichtsänderungen.
- Erste Verluste.
- Spätere Verluste.

Die Stichprobe darf nicht nachträglich nach erfolgreicher Wiederherstellbarkeit gefiltert werden.

### C2 – Zwei vollständige Zustände

Für jedes Verlustereignis existieren:

**Zustand A:** Vor der Erfahrung, nach der die zuvor korrekte Antwort verschwindet.

**Zustand B:** Nach dieser Erfahrung, mit nicht mehr korrektem autonomen Abruf.

Beide Zustände sind vollständig zu erfassen.

### C3 – Ausgangskontrollen

Vor jeder Zerlegung:

1. Zustand A liefert die erwartete korrekte Antwort.
2. Zustand B liefert die erwartete nicht korrekte Antwort.
3. Identische Kopien von A liefern identische Ergebnisse.
4. Identische Kopien von B liefern identische Ergebnisse.
5. Die vollständige bisherige T-Rückübernahme reproduziert den B.5.2-Befund.

Wenn eine dieser Bedingungen nicht erfüllt ist, darf der Fall nicht stillschweigend als gültiger Kausalnachweis verwendet werden.

Er ist gesondert zu dokumentieren.

### C4 – Identische Prüfbedingungen

Die Versuche müssen nach Möglichkeit sicherstellen, dass die diagnostischen Unterschiede tatsächlich aus der untersuchten Zustandskomponente stammen.

Insbesondere sind zu kontrollieren:

- Sensorischer Reiz.
- Zielzuordnung.
- Ausgangsgewichte.
- Gültigkeit neuronaler Zustände.
- Ereigniswarteschlange.
- Simulierte Zeit beziehungsweise definierter Zeitbezug.
- Zusätzliche Modulation.
- Zusätzliche Exploration.
- Weitere Plastizität.

Eine alte Runtime und eine neue Runtime dürfen nicht ungeprüft verglichen werden, wenn sie durch unterschiedliche Zeit- oder Ereignisbedingungen verschiedene Antworten erzeugen könnten.

---

## 7. Phase D – Isolierte Wiederherstellung einzelner Komponenten

Dieser Abschnitt ist der Hauptversuch von B.5.3.

### D1 – Grundverfahren

Verwende Zustand B als Ausgangspunkt.

Übernimm jeweils nur eine definierte Zustandskomponente aus Zustand A.

Alle übrigen Zustände bleiben unverändert.

Präsentiere anschließend denselben sensorischen Reiz.

Untersuche:

- Wird die korrekte autonome Antwort wiederhergestellt?
- Bleibt sie falsch?
- Entsteht keine Ausgabe?
- Entstehen Doppelantworten?
- Verändert sich die Antwort des anderen Reizes?

### D2 – Einzelkomponenten

Untersuche separat:

- Alte Membranpotenziale.
- Alte Eingangsereigniszähler.
- Altes G3-Ereignisgedächtnis.
- Alte Eligibility-Spuren.
- Alte synaptische Aktivitätszustände.
- Alte Erholungszustände.
- Alte zeitabhängige Zustände, soweit isoliert gültig.
- Weitere relevante Runtime-Felder.

### D3 – Umgekehrte Intervention

Führe auch den Gegenversuch durch.

Verwende Zustand A und übernehme ausschließlich die jeweilige neue Komponente aus Zustand B.

Prüfe, ob dadurch der zuvor korrekte Abruf verloren geht.

Damit sollen zwei Aussagen getrennt werden:

**Wiederherstellung:** Eine alte Komponente stellt im neuen Zustand eine richtige Antwort her.

**Verlustauslösung:** Eine neue Komponente bewirkt im früheren Zustand eine nicht korrekte Antwort.

Beide Richtungen müssen untersucht werden.

### D4 – Technische Zustandsgültigkeit

Nicht jede interne Runtime-Komponente lässt sich gefahrlos unabhängig transplantieren.

Beispielsweise könnten Zeitangaben, Ereignisgenerationen oder lokale Erinnerungen voneinander abhängen.

Deshalb gelten:

- Keine ungültigen künstlichen Zustände.
- Keine unbemerkte Veränderung weiterer Komponenten.
- Keine automatischen Resets.
- Keine neue Lernfunktion.
- Vollständige Dokumentation aller technisch gekoppelten Felder.

Falls isolierte Transplantationen nur durch experimentelle Diagnoseschnittstellen möglich sind, dürfen diese eingerichtet werden.

Sie müssen vom normalen neuronalen Verhalten getrennt und abschaltbar bleiben.

Ist eine isolierte Intervention technisch nicht sinnvoll möglich, ist dies ausdrücklich als Einschränkung festzuhalten.

---

## 8. Phase E – Notwendigkeit, Hinreichendheit und Wechselwirkungen

### E1 – Notwendigkeit

Ein Zustand gilt innerhalb eines untersuchten Falles als notwendig für die beobachtete Abrufstörung, wenn seine gezielte Rücknahme unter ansonsten kontrollierten Bedingungen die Störung beseitigt.

### E2 – Hinreichendheit

Ein Zustand gilt innerhalb eines untersuchten Falles als hinreichend, wenn seine isolierte Übernahme in den zuvor korrekten Ausgangszustand die Störung reproduziert.

### E3 – Kombinationen

Wenn einzelne Komponenten keinen hinreichenden Effekt zeigen, sind Kombinationen zu prüfen.

Priorität haben:

1. Membranpotenzial + G3.
2. Membranpotenzial + Eingangsaktivität.
3. Membranpotenzial + synaptische Aktivitätszustände.
4. G3 + Eligibility.
5. Membranpotenzial + G3 + Eligibility.
6. Weitere Kombinationen, die sich aus der technischen Wirkungsanalyse ergeben.

Es ist nicht erforderlich, blind sämtliche Kombinationen beliebig vieler Zustandsfelder zu testen.

Die Auswahl muss aus den tatsächlichen Codeabhängigkeiten und Einzelversuchen begründet werden.

### E4 – Zustandsreduzierung

Wenn eine Kombination eine Abrufstörung zuverlässig reproduziert, reduziere sie schrittweise auf die kleinste kausal notwendige Menge lokal verfügbarer Zustände.

### E5 – Wechselwirkungen mit Synapsengewichten

Wiederhole ausgewählte transiente Einzel- und Kombinationseingriffe bei:

- Unveränderten neuen Gewichten.
- Früheren sensorischen Gewichten.
- Früheren inhibitorischen Gewichten.
- Vollständig früheren Gewichten.

Damit soll geklärt werden, ob ein transienter Faktor nur bei einer bestimmten synaptischen Konfiguration wirksam ist.

---

## 9. Phase F – Der Probeimpuls als Zustandsveränderung

B.5.2 zeigte:

- 15.194/15.194 identische Runtime-Kopien liefern dieselbe Diagnose.
- Zwei aufeinanderfolgende Probeimpulse auf derselben Runtime liefern nur in 267/15.194 Fällen dieselbe Antwort.

Die Ursachen dieser Differenz müssen isoliert werden.

### F1 – Identische Anfangszustände

Erzeuge mehrere vollständige Kopien desselben neuronalen Zustands.

Jede Kopie erhält genau einen identischen sensorischen Probeimpuls.

Die Ergebnisse müssen deterministisch übereinstimmen.

### F2 – Aufeinanderfolgende Impulse

Präsentiere denselben Reiz mehrfach auf einer fortgesetzten Runtime.

Erfasse nach jeder Präsentation:

- Motorische Antwort.
- Membranpotenziale.
- G3-Zustand.
- Eligibility.
- Eingangsaktivität.
- Synaptische Aktivitätszustände.
- Simulierte Zeit.
- Ereignisanzahl.

### F3 – Zeitliche Abstände

Prüfe vorab definierte Abstände zwischen Probeimpulsen:

- 0 Zeiteinheiten.
- 1 Zeiteinheit.
- 2 Zeiteinheiten.
- 4 Zeiteinheiten.
- 8 Zeiteinheiten.
- 16 Zeiteinheiten.
- 32 Zeiteinheiten.
- 64 Zeiteinheiten.
- Gegebenenfalls längere Intervalle, falls technisch begründet.

Diese Zeitabstände dürfen nicht durch globale neuronale Ticks simuliert werden.

Es bleibt bei der vorhandenen ereignisgesteuerten Verarbeitung.

### F4 – Unterschiedliche Reize

Untersuche die folgenden Sequenzen:

- X → X.
- Y → Y.
- X → Y.
- Y → X.
- X → Y → X.
- Y → X → Y.

Prüfe, wie sich die Aktivität des jeweils ersten Reizes auf die nachfolgende autonome Antwort auswirkt.

### F5 – Keine zusätzliche Plastizität

Die Probeversuche dürfen keine neue synaptische Lernänderung erzeugen.

Der Einfluss der Reizverarbeitung selbst muss isoliert bleiben.

### F6 – Kontrollierte Probewiederholung

Unterscheide:

**Prüfung aus frischer Kopie:** Jede Antwort entsteht aus demselben Ausgangszustand.

**Fortgesetzte Prüfung:** Jede Antwort entsteht aus dem durch vorherige Prüfungen veränderten Zustand.

Beide Ergebnisse müssen getrennt dokumentiert werden.

---

## 10. Phase G – Lokale Zustandsdynamik

Auf Grundlage der vorherigen Versuche soll die konkrete neuronale Dynamik untersucht werden.

### G1 – Membranverhalten

Falls Membranpotenziale einen nachweisbaren Beitrag leisten:

- Welche Eingänge verändern sie?
- Welche Werte bleiben nach einem Feuerereignis bestehen?
- Welche Bedingungen lösen einen Reset aus?
- Welche Bedingungen verhindern erneutes Feuern?
- Welche Rolle spielt inhibitorische Integration?
- Wie verändert sich die Aktivität beim nächsten Reiz?

### G2 – Lokales Ereignisgedächtnis

Falls G3 einen Beitrag leistet:

- Welche Ereignisse werden gespeichert?
- Welche Einträge werden später gelesen?
- Welche Informationen sind lediglich für Plastizität relevant?
- Beeinflusst die Ereignisgeschichte eine aktuelle Feuerentscheidung unmittelbar?
- Wie verändern konkurrierende Erfahrungen den lokalen Speicher?

### G3 – Eingangs- und Synapsenaktivität

Falls Aktivitätszustände einen Beitrag leisten:

- Handelt es sich um verhaltenswirksame Zustände?
- Werden sie als Bedingungen für neue Feuerereignisse verwendet?
- Werden sie ausschließlich protokolliert?
- Welche lokale Bedeutung besitzen sie für weitere Verarbeitung?

### G4 – Zeitliche Verarbeitung

Untersuche den Zusammenhang zwischen:

- Impulsankunft.
- Membranintegration.
- Feuerereignis.
- Lokaler Zustandsänderung.
- Nächstem sensorischen Impuls.

Die Diagnose muss die tatsächliche Ereignisreihenfolge berücksichtigen.

### G5 – Biologische Einordnung

Beschreibe die identifizierten Zustandswirkungen im Verhältnis zu plausiblen biologischen Mechanismen.

Die biologische Analogie darf jedoch keinen experimentellen Nachweis ersetzen.

---

## 11. Phase H – Auswirkungen auf zwei erlernte Beziehungen

Alle wichtigen Kausalexperimente müssen beide Reize berücksichtigen.

### H1 – Einzelne Bindung

Erzeuge einen Zustand, in dem X autonom korrekt ist.

Untersuche, ob transiente Zustandsänderungen X vorübergehend nicht abrufbar machen.

### H2 – Zweite Bindung

Wiederhole den Versuch für Y.

### H3 – Gleichzeitig korrekte Bindungen

Identifiziere Zustände, in denen X und Y gleichzeitig autonom korrekt sind.

Untersuche:

- Veränderungen durch weitere natürliche Erfahrungen.
- Veränderungen durch einzelne Probeimpulse.
- Verlust nur einer Beziehung.
- Verlust beider Beziehungen.
- Wiederherstellung einzelner oder beider Beziehungen.

### H4 – Selektive Wiederherstellung

Wenn eine transiente Zustandsintervention X wiederherstellt, muss geprüft werden, ob Y erhalten bleibt.

Eine Verbesserung von X bei gleichzeitiger Zerstörung von Y ist kein Nachweis verbesserter gemeinsamer Gedächtnisstabilität.

### H5 – Rollenunabhängigkeit

Die Ergebnisse müssen für normale und inverse Zielzuordnungen gelten.

Es darf keine technisch bevorzugte Sensor- oder Motorrolle entstehen.

---

## 12. Phase I – Bedingte minimale Forschungsvariante

**Es wird zunächst kein neuer neuronaler Mechanismus implementiert.**

Erst wenn eine lokale Ursache isoliert wurde, darf höchstens eine minimal begründete Forschungsvariante untersucht werden.

### Voraussetzungen

Erforderlich sind:

1. Reproduzierbarer Abruffehler.
2. Identifizierte lokale Zustandskomponente oder begrenzte Interaktion.
3. Nachgewiesene kausale Wirkung.
4. Reproduktion in unabhängigen Seeds.
5. Nachweis, dass die vorgeschlagene Änderung nicht lediglich einen diagnostischen Zustand künstlich ersetzt.
6. Erhalt der zweiten Reizbeziehung.
7. Erhalt der Veränderbarkeit bei neuer Umweltbewertung.

### Mögliche Forschungsrichtung

Falls beispielsweise ein bestimmter Membranzustand nach einer neuronalen Aktivität eine erneute korrekte Reaktion verhindert, darf untersucht werden, ob bereits vorhandene lokale Ereignis- oder Erholungsmechanismen diesen Zustand angemessen verarbeiten.

Die konkrete Umsetzung darf erst aus den Ergebnissen abgeleitet werden.

### Nicht zulässig

- Globaler Membranreset nach jedem Reiz.
- Technische Wiederherstellung eines früher erfolgreichen Gesamtzustands.
- Externe Auswahl korrekter Motorneuronen.
- Rücksetzen aller Eligibility-Spuren ohne kausale Begründung.
- Automatischer Schutz aller einmal erfolgreichen Gewichte.
- Globale Winner-take-all-Auswahl.
- Verwendung externer Reizsemantik.
- Technisches Unterdrücken unerwünschter Antworten.
- Pauschale Änderung der Feuergrenze.
- Pauschale Änderung der Lernschrittweite.

Eine bessere Trefferquote durch künstliche Zustandsnormalisierung allein ist keine ausreichende Begründung für eine Produktionsfunktion.

---

## 13. Kontrollgruppen

Mindestens:

| Gruppe | Beschreibung |
|---|---|
| K0 | Unveränderte B.5.2-Referenz |
| K1 | No-Learning |
| K2 | Feste inhibitorische Verbindungen |
| K3 | Lernbare inhibitorische Verbindungen |
| K4 | Keine zusätzliche Plastizität |
| K5 | Isolierte Membranzustands-Intervention |
| K6 | Isolierte G3-Intervention |
| K7 | Isolierte Eligibility-Intervention |
| K8 | Isolierte Aktivitätszustands-Intervention |
| K9 | Kontrollierte transiente Kombinationen |
| K10 | Probe- und Zeitabstandskontrollen |
| K11 | Bedingte minimale Forschungsvariante |
| K12 | Identischer Aufbau mit deaktivierter neuer Funktion |

K11 und K12 werden nur implementiert, falls die Ursachenanalyse eine neue Funktion tatsächlich begründet.

Alle anderen gezielten Zustandsinterventionen sind diagnostische Kontrollen.

Sie dürfen keine neue Produktionslernregel darstellen.

---

## 14. Lernaufgaben

Die bisherigen Aufgaben bleiben bestehen:

1. Einzelreiz X→A.
2. Einzelreiz Y→B.
3. Sequenziell X→A, danach Y→B.
4. Sequenziell Y→B, danach X→A.
5. Streng alternierende Erfahrungen.
6. Zufällig gemischte Erfahrungen.
7. Inverse Zuordnung X→B und Y→A.
8. Umweltumkehr ohne Zustandsreset.
9. Retention ohne weitere Plastizität.

Zusätzlich verpflichtend:

- Wiederherstellung eines transient gestörten Abrufs.
- Verlustauslösung durch isolierte transiente Änderung.
- Untersuchung zweier aufeinanderfolgender gleicher Reize.
- Untersuchung aufeinanderfolgender unterschiedlicher Reize.
- Kausale Wiederherstellung ohne Gewichtsänderung.
- Gleichzeitiger Erhalt zweier autonomer Beziehungen.

---

## 15. Primäre Messgrößen

### E1 – Autonomer Erfolg

E1 bleibt die primäre Lernmetrik.

Die autonome Antwort wird ohne zusätzliche Exploration und ohne weitere Plastizität aus einer vollständigen Runtime-Kopie gemessen.

### E2 – Explorationsgestützter Erfolg

E2 wird separat aus einer identischen Ausgangskopie ausgewertet.

### B.5.3-spezifische Messgrößen

Erforderlich sind:

- Anzahl kausal durch Membranpotenziale erklärter Abrufstörungen.
- Anzahl kausal durch G3 erklärter Abrufstörungen.
- Anzahl durch weitere lokale Zustandskomponenten erklärter Störungen.
- Anzahl nur durch Kombination erklärbarer Störungen.
- Anzahl technisch nicht isolierbarer Fälle.
- Anteil notwendiger und hinreichender Zustandsfaktoren.
- Veränderung der Antwort durch einen einzelnen Probeimpuls.
- Einfluss definierter Reizabstände.
- Wiederherstellung beider Beziehungen.
- Veränderung des anderen Reizes bei isolierter Intervention.
- Reproduktion über unabhängige Seeds.

Eine diagnostisch wiederhergestellte Antwort zählt nicht als neue erlernte Fähigkeit.

---

## 16. Statistische Versuchsgestaltung

### Entdeckung

Mindestens:

- 64 gepaarte Seeds.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnungen.
- Reproduzierbare neuronale Ausgangszustände.

### Bestätigung

Mindestens 64 zusätzliche, bislang unbenutzte Seeds je Stromfamilie und Zielzuordnung.

Die unabhängigen Bestätigungsdaten dürfen nicht zur Entwicklung oder Auswahl einer neuen Regel verwendet werden.

### Stichproben

Die ausgewählten Verlustgruppen sind getrennt auszuwerten:

- Alle Verluste ohne Gewichtsänderung.
- Erste und zweite Verluste.
- Verluste mit synaptischen Änderungen.
- Verluste mit kombinierten Ursachen.

Die Ergebnisse dürfen nicht ungewichtet auf alle 66.159 Verluste verallgemeinert werden.

### Auswertung

Erforderlich sind:

- Gepaarte Kausalvergleiche.
- Seedbezogene Erfolgsquoten.
- Clusterbewusste Bootstrap-Konfidenzintervalle.
- Einzelkomponenten-Matrizen.
- Kombinationsmatrizen.
- Zeit- und Probeeffekte.
- Verwechslungsmatrizen.
- Vollständige relevante Zustandsdifferenzen.
- Gesonderte Auswertung ungültiger Zustandskombinationen.
- Unabhängige Bestätigung.

Mehrere Verlustereignisse desselben Netzes sind statistisch nicht als unabhängige Netzwerke zu behandeln.

---

## 17. Allgemeine Erfolgskriterien

Für eine neu vorgeschlagene neuronale Funktion gelten weiterhin:

- Mindestens 75 % E1 für X.
- Mindestens 75 % E1 für Y.
- Mindestens 48/64 Seeds bestehen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Lernvorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Unabhängige Bestätigung.
- Reproduktion über drei Zufallsstromfamilien.
- Normale und inverse Rollen.
- Erhalt beider Beziehungen.
- Retention ohne weitere Plastizität.
- Weiterhin mögliche Umweltanpassung.

### Spezifischer Forschungserfolg B.5.3

B.5.3 gilt wissenschaftlich als erfolgreich, wenn:

1. Die einzelnen transienten Zustandskomponenten technisch vollständig klassifiziert wurden.
2. Verhaltenswirksame Zustände von reinen Diagnosezählern getrennt wurden.
3. Mindestens ein wesentlicher transienter Ursachenanteil kausal eingegrenzt wurde oder nachvollziehbar gezeigt wurde, weshalb eine weitere Zerlegung nicht möglich ist.
4. Der Einfluss des Probeimpulses isoliert wurde.
5. Einzelne und kombinierte Zustandswirkungen voneinander getrennt wurden.
6. Der Einfluss auf die zweite erlernte Beziehung dokumentiert ist.
7. Die Ergebnisse mit unabhängigen Seeds überprüft wurden.

Ein negativer Befund bezüglich allgemeiner Lernleistung ist zulässig.

---

## 18. Architekturgrenzen

Unverändert gelten:

- C++17.
- Vollständig ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokales G3-Ereignisgedächtnis.
- Zeitlich begrenzte Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine semantischen Reizkennungen im neuronalen Lernzustand.
- Keine technisch vorgegebene richtige Handlung.
- Keine globale Handlungswahl.
- Keine automatische Löschung erlernter Gewichte.
- Keine permanenten Zeitstempel pro Synapse.
- Keine globale neuronale Zustandsabfrage für die Entscheidungsfindung.
- Vollständige Persistenz.
- Deterministische Verarbeitung.

Synapsenwachstum bleibt in den Hauptversuchen deaktiviert.

Diagnostische Zustandsmanipulationen sind ausdrücklich keine Produktionsmechanismen.

---

## 19. Technische Validierung

Alle bisherigen 525 Kerntests müssen weiterhin bestehen.

Zusätzlich sind Tests erforderlich für:

- Vollständige transiente Zustandsinventur.
- Identische Gewichte bei verschiedenen Membranpotenzialen.
- Identische Membranpotenziale bei verschiedenen G3-Zuständen.
- Isolierte Eligibility-Änderungen.
- Reine Diagnosezähler ohne Verhaltenswirkung.
- Zustandskopien mit identischen Anfangsfingerprints.
- Notwendige und hinreichende Einzelinterventionen.
- Kombinierte transiente Zustände.
- Gültige und ungültige Transplantationsversuche.
- Einfluss wiederholter Probeimpulse.
- Zeitliche Trennung von Reizen.
- Unterschiedliche Reizreihenfolgen.
- Gleichzeitiger Erhalt beider Beziehungen.
- Persistenz neuer Diagnoseschnittstellen, sofern erforderlich.
- Deterministische Fortsetzung.
- Keine Änderung bisheriger Lernmechanismen.

Ungültige neuronale Zustände dürfen nicht als erfolgreiche Forschungsresultate gezählt werden.

---

## 20. Lieferumfang

Nova-EVE erstellt:

1. Technische Inventur sämtlicher transienter Runtime-Zustände.
2. Reproduktion der B.5.2-Referenz.
3. Getrennte Verlustgruppen nach Ursache und Stichprobenauswahl.
4. Kontrollierte Zustandskopie-Experimente.
5. Einzelablationen aller relevanten transienten Komponenten.
6. Kombinierte Kausalexperimente.
7. Technische Wirkungsanalyse des lokalen G3-Zustands.
8. Untersuchung von Membranpotenzialen und Eingangsaktivität.
9. Untersuchung des Einflusses wiederholter Probeimpulse.
10. Experimente mit definierten Reizabständen und Reizfolgen.
11. Prüfung beider gleichzeitig erlernter Beziehungen.
12. Gegebenenfalls eine minimal begründete lokale Forschungsvariante.
13. Unabhängige Bestätigungsversuche.
14. Vollständige Rohdaten und statistische Auswertungen.
15. Regressionstests.
16. Persistenz- und Stabilitätsprüfung.
17. Forschungsbericht B.5.3.
18. Entscheidungsvorlage für B.5.4.

Historische Forschungsdaten bleiben unverändert erhalten.

---

## 21. Abschlussfragen

Am Ende muss beantwortet werden:

**A.** Welche transienten neuronalen Zustände beeinflussen den autonomen Abruf tatsächlich?

**B.** Welche gespeicherten Runtime-Felder dienen lediglich der Diagnostik?

**C.** Welcher Zustand kann eine zuvor korrekte Antwort bei identischen Synapsengewichten verhindern?

**D.** Kann die Rücknahme genau dieses Zustands die korrekte Antwort wiederherstellen?

**E.** Sind mehrere transiente Zustände gemeinsam notwendig?

**F.** Welche Rolle spielen Membranpotenziale und lokales G3-Ereignisgedächtnis?

**G.** Beeinflussen Eligibility-Spuren den autonomen Abruf unmittelbar oder ausschließlich spätere Lernvorgänge?

**H.** Warum verändert bereits ein Probeimpuls die Antwort auf den nächsten Probeimpuls?

**I.** Welche Bedeutung hat der zeitliche Abstand zwischen zwei sensorischen Ereignissen?

**J.** Lassen sich beide erlernten Beziehungen gleichzeitig wiederherstellen, ohne synaptische Gewichte zu verändern?

**K.** Gibt es einen lokal begrenzten, biologisch plausiblen Mechanismus, der eine nachgewiesene Abrufstörung verhindern könnte, ohne natürliche Lernfähigkeit einzuschränken?

**L.** Welche Experimente sind vor einer möglichen Implementierung eines solchen Mechanismus noch notwendig?

Eine neue neuronale Funktion darf nur aus einem isolierten, reproduzierbaren Kausalbefund abgeleitet werden.

---

## 22. Forschungsleitlinie

**Eine gespeicherte neuronale Veränderung und ihre momentane Nutzung sind zwei unterschiedliche Eigenschaften.**

Ein neuronales Netzwerk kann die für eine Fähigkeit notwendigen Verbindungen besitzen, ohne unter jedem aktuellen Zustand dieselbe Antwort zu erzeugen.

Das muss nicht zwangsläufig bedeuten, dass die zugrunde liegende Information verloren gegangen ist.

Ebenso bedeutet ein erfolgreicher einzelner Abruf noch nicht, dass die Fähigkeit dauerhaft verfügbar ist.

Für EVE wollen wir deshalb verstehen, wie lokale neuronale Zustände:

- Vorherige Aktivität beeinflussen.
- Neue Reize verarbeiten.
- Vorhandene Verbindungen wirksam machen.
- Konkurrierende Aktivität ermöglichen oder verhindern.
- Bereits erworbene Fähigkeiten verfügbar halten.
- Neue Erfahrungen weiterhin zulassen.

Es darf keine technische Vorgabe der richtigen Handlung entstehen.

**B.5.3 soll die kleinsten kausal wirksamen Bestandteile transienter Abrufstörungen identifizieren. Erst danach darf entschieden werden, ob EVE überhaupt einen zusätzlichen lokalen Gedächtnis- oder Abrufmechanismus benötigt.**
