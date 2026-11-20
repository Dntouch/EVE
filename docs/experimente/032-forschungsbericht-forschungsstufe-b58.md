# Forschungsbericht – Forschungsstufe B.5.8

## Ergebnis

B.5.8 bestätigt die leitende Unterscheidung:

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand.**

Bereits erworbene Beziehungen blieben in der untersuchten Stichprobe ohne
weitere Gewichtsschreibungen stabiler als bei fortlaufender Plastizität. Nach
512 identischen weiteren Erfahrungen erreichten die gesperrten Kopien im
Mittel 95,83 % E1 für X und 97,92 % für Y; 90/96 waren gleichzeitig robust.
Mit aktiver Plastizität lagen die Werte bei 85,48 % und 79,17 % sowie 62/96
robusten Netzen.

Die Sperre erhält allein jedoch keine Anpassungsfähigkeit. Nach umgekehrter
Umweltbewertung erreichten weiterhin gesperrte Netze 0 % E1. Durchgehend aktive
Netze erreichten 82,19 %/80,21 %, nach der stabilen Phase reaktivierte Netze
79,10 %/84,38 %. Reaktivierung und durchgehend aktive Plastizität unterschieden
sich nicht statistisch abgesichert.

Dieser Befund rechtfertigt keine automatische Stabilitätsregel. Er zeigt, dass
vorhandene neuronale Struktur eine Fähigkeit ohne Dauerplastizität tragen kann,
während Gewichtsänderungen für eine neue Umweltbeziehung weiterhin notwendig
sind.

## Zentrale Befunde

### Reproduktion und unabhängige Bestätigung

Die B.5.7-Referenz wurde exakt reproduziert: 242/384 robuste Discovery-Netze
und 254/384 robuste Bestätigungsnetze. Neue B.5.8-Ströme ergaben 240/384 und
244/384 robuste Netze. Alle untersuchten Fest-16-Netze hatten beide Beziehungen
mindestens zeitweise erworben.

Die Robustheit bleibt stark zeitabhängig: Fest-1 und Fest-12 erreichen 0/384,
Fest-32 259/384 und unregelmäßige Abstände 156/384. R2-50 und S5-aus erreichen
je 73/384; No-Learning 0/384.

### Lernen und Nichtlernen

P0 führte in 49.152 Erfahrungen 19.350 positive und 19.351 negative
Gewichtsänderungen aus. 26.013 Erfahrungen veränderten kein Gewicht. P1
verarbeitete dieselben 49.152 Erfahrungen ohne tatsächliche Änderung; 27.777
lokale Schreibversuche wurden isoliert unterdrückt, Modulationsroute und
Eligibility-Verbrauch blieben aktiv.

Die stabile P1-Phase widerlegt die Annahme, eine vorhandene Fähigkeit müsse
durch fortwährende synaptische Änderung aufrechterhalten werden. Die
Umkehrphase widerlegt zugleich die Annahme, stabiler Erhalt und aktive
Lernfähigkeit seien dasselbe.

### Fehlende Handlung

Negative Modulation nach einer Nichtantwort ist nicht generell schädlich. In
96 gepaarten Fällen erreichte die natürliche Kopie unmittelbar 64,58 % für X
und 66,67 % für Y, die Kopie ohne Gewichtsschreiben nur 34,38 % und 41,67 %.

Eine Nichtantwort identifiziert aber keine konkrete zu schwächende Synapse.
Hemmungsentfernung, Membran-Nullsetzung und deren Kombination korrigierten
keinen der 96 M3-Zustände unmittelbar. Die zeitliche Eligibility bleibt damit
eine Kandidatenberechtigung, kein Beweis kausaler Verantwortung.

### Neutrales und fehlendes Feedback

F1 ohne Feedback und F2 mit neutralem Feedback änderten beide kein Gewicht und
zeigten im festgelegten Folgepunkt dieselbe Abrufbarkeit. Nur F2 durchlief
jedoch 20 Modulationsimpulse je Fall. F5 durchlief ebenfalls die Route und
verbrauchte die Eligibility, unterdrückte aber im Mittel 2,52 Schreibversuche.
Nullveränderung besitzt deshalb verschiedene technische Ursachen, auch wenn
der kurzfristige Funktionswert gleich sein kann.

### Positive Konkurrenz

Von 31.605 beobachteten positiven Kandidatenanwendungen waren 1.362 unmittelbar
nützlich, 3.175 unmittelbar schädlich und 27.068 in den festgelegten Proben
neutral. Darin sind nun auch technisch abgewiesene Anwendungen an
Gewichtsgrenze oder Eingangsbudget vollständig erfasst. Positive Verstärkung
kann eine Gegenbeziehung kausal beeinträchtigen.
Eine pauschale Unterdrückung wäre dennoch unzulässig, weil positive Änderungen
auch Fähigkeiten erwerben und verbessern.

### Lokale Information

20 von 29 detaillierten lokalen Signaturen kamen in unterschiedlichen globalen
Funktionsklassen vor. Die vorhandene Diagnose findet daher keine sichere
ausschließlich lokale Grenze zwischen notwendiger, neutraler und schädlicher
Änderung. Es wird keine neue Lernregel eingeführt.

## Hypothesenentscheidung

- **H1 bestätigt:** Fähigkeiten blieben über 512 Erfahrungen ohne weitere
  Gewichtsschreibungen verfügbar.
- **H2 bestätigt:** Nach Reaktivierung lernte dieselbe Population die
  umgekehrte Umweltbeziehung; P4 war P2 statistisch gleichwertig.
- **H3 bestätigt:** Ein großer Anteil der Änderungen besaß keinen
  unmittelbaren Funktionsgewinn; P1 stabilisierte besser als P0.
- **H4 bestätigt:** Natürliche negative Plastizität war F5 in der
  Nichtantwort-Fortsetzung deutlich überlegen und für Umkehrlernen notwendig.
- **H5 teilweise bestätigt:** Nichtantworten bilden verschiedene Aktivitäts-
  und Explorationsklassen. Keine der einfachen lokalen Einzelursachen erklärte
  die 96 M3-Fälle ausreichend.
- **H6 bestätigt:** Eligibility ist zeitlich berechtigt, aber kausal
  mehrdeutig.
- **H7 bestätigt:** Positive Konkurrenz erzeugte sowohl Nutzen als auch
  unmittelbare Verluste anderer Beziehungen.
- **H8 technisch bestätigt, funktional nicht getrennt:** Fehlendes, neutrales
  und schreibgesperrtes Feedback besitzen verschiedene Ereignis- und
  Eligibility-Abläufe, F1/F2/F5 zeigten im gemessenen Folgepunkt aber dieselbe
  Abrufrate.
- **H9 gestützt:** 20/29 erfasste lokale Signaturen waren global mehrdeutig.
  Eine vollständige Unmöglichkeit jeder künftigen lokalen Information ist
  damit nicht bewiesen.
- **H10 bestätigt:** Fest-1, Fest-12, Fest-16, Fest-32 und unregelmäßige
  Abstände unterscheiden sich stark.

## Antworten A–Q

**A. Muss eine Erfahrung eine synaptische Veränderung auslösen?** Nein. In P1
wurden 49.152 Erfahrungen mit Aktivität, Exploration, Modulation und
Eligibility verarbeitet, ohne ein Gewicht zu verändern.

**B. Können Fähigkeiten ohne fortwährende Änderungen stabil bleiben?** Ja. P1
blieb über 512 Erfahrungen stabiler als P0.

**C. Bleibt das Netzwerk später lernfähig?** Ja, wenn die Plastizität wieder
aktiviert wird. P4 erlernte die Umkehr; P3 mit weiter bestehender Sperre nicht.

**D. Welche Änderungen sind für den Erhalt erforderlich?** Für die untersuchte
stabile Phase ist keine weitere Änderung als notwendig nachgewiesen. Das gilt
nicht für neue Umweltbeziehungen.

**E. Welche Änderungen besitzen keinen Funktionsnutzen?** 29.536 von 35.391
lokalen Kandidatenbeobachtungen änderten die unmittelbaren Vorher-/Nachher-Prüfungen
nicht. Langfristige Nutzlosigkeit ist daraus nicht ableitbar.

**F. Weshalb folgen Nichtantworten so viele negative Ereignisse?** Eine
Nichtantwort wird extern negativ bewertet; die neuronale Modulationsroute
erreicht zeitlich aktive Eligibility. Diese lokale Spur enthält keine Aussage,
welcher Faktor die Nichtantwort verursacht hat.

**G. Welche Nichtantworten werden sinnvoll korrigiert?** Natürliche negative
Modulation verbessert viele Fortsetzungen, aber B.5.8 findet keine lokal
sichere Einzelklassifikation. Die einfachen Hemmungs-/Membrankopien erklären
keinen M3-Fall vollständig.

**H. Wann ist neutrales oder fehlendes Feedback sinnvoll?** Als getrennte
Kausalkontrollen. Eine allgemeine neuronale Entscheidung für Neutralität ist
nicht gerechtfertigt.

**I. Worin unterscheiden sich F1 und F2?** F1 erzeugt kein Ereignis. F2 läuft
über die Modulationsroute, verändert und verbraucht Eligibility aber nicht.

**J. Welche Spuren bleiben erhalten?** F1 und F2 lassen die Eligibility bis
zum natürlichen Ablauf bestehen; F5 verbraucht sie wie wirksame Modulation.

**K. Wie stark wirkt positive Konkurrenz?** 3.175 positive Beobachtungen waren
unmittelbar schädlich, 1.362 nützlich und 27.068 kurzfristig neutral.

**L. Welche Rolle spielt Redundanz?** Einzelbeiträge können neutral erscheinen,
obwohl kombinierte Kandidaten wirksam sind. Deshalb wird Neutralität nicht mit
Irrelevanz gleichgesetzt.

**M. Sind die zwölf historischen Fälle erklärt?** Nein. Getrennte positive,
negative und vollständige Schreibsperren nach dem letzten gemeinsamen Zustand
änderten ihre Endklasse nicht. Sie bleiben offen.

**N. Welche Unterschiede sind lokal erkennbar?** Polarität, Wirkung, Gewicht,
Aktivität, Membran, Eligibility-Stärke, Alter und Umfang sind lokal verfügbar
und korrelieren teilweise mit Folgen.

**O. Wo reichen die Informationen nicht aus?** Identische erfasste lokale
Signaturen gehören häufig zu nützlichen, neutralen und schädlichen globalen
Folgen. Externe Kausalnotwendigkeit ist lokal nicht vorhanden.

**P. Ist eine neue Lernregel notwendig?** Für den wissenschaftlichen Abschluss
nein; für eine Produktionsregel fehlt ein eindeutig lokaler, unabhängig
bestätigter Selektor.

**Q. Was folgt für B.5.9?** B.5.9 soll untersuchen, ob lokale
Konkurrenzdynamik vor der Modulation – insbesondere relative gleichzeitige
Beiträge und postsynaptische Entscheidungsmarge – zusätzliche Information
liefert, ohne gelernte Gewichte zu schützen oder globale Erfolgslabels zu
verwenden.

## Grenzen

- Die Stabilitätsstichprobe umfasst 96 präregistriert ausgewählte Zustände,
  nicht alle 768 neuen Netze.
- Die schmale Population `near_robust` kam in den neuen Matrizen nicht vor.
- Kurzfristig neutrale Änderungen können später wirksam werden.
- Fest-1 ist eine kurze, aber weiterhin getrennt bewertete Folge; echte
  gleichzeitig überlappende Erfahrungsbewertungen wurden nicht untersucht.
- Die lokale Mehrdeutigkeit gilt für das erfasste Inventar und ist kein
  allgemeiner Unmöglichkeitsbeweis.

## Merge-Empfehlung

**B.5.8 nach `main` mergen: Ja.**

Die Forschung ist reproduzierbar, 665/665 Tests bestehen, Persistenz und
Determinismus sind erhalten, alle Zustände sind gültig und die neue
Schreibsperre bleibt standardmäßig deaktivierte Diagnostik. S4 bleibt im
Produktionsstandard aus. Es wird keine neue Produktions-Lernregel freigegeben.

