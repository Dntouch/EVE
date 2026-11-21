# Forschungsbericht – Forschungsphase C.1

## Methodisch korrigierter Abschluss

**Datum:** 21. November 2026

**Status:** nachträgliche methodische Korrektur abgeschlossen; keine neue
Produktions-Lernregel freigegeben

## Korrekturhinweis

Der ursprüngliche C.1-Lauf setzte W0 und W1 erst bei der Gewichtsschreibung
nach der Motorhandlung auseinander. Aus ihrer identischen Handlung durfte
daher keine Neutralität der beteiligten Synapse abgeleitet werden. Ebenso
belegten andere starke Eingänge und einzelne Kontrollpunkte weder eine
funktionale Reorganisation noch kontinuierliche Funktionslosigkeit.

Die ursprünglichen Daten unter `research/results/c1` bleiben unverändert. Die
post-hoc ergänzten H0–H3-Interventionen, Ablationen alternativer Eingänge und
Schritt-für-Schritt-Diagnosen liegen getrennt unter
`research/results/c1/method-correction`.

## Korrigierter Kurzbefund

- Alle 38 ausgewählten Synapsen waren an der fehlerhaften Motorentscheidung
  kausal beteiligt: H0 erzeugte 38/38-mal eine falsche Doppelantwort, H1 mit
  vor der Erfahrung deaktivierter Synapse 38/38-mal die korrekte Einzelantwort.
  H2 reproduzierte H0 in 38/38 Fällen exakt.
- Die Abschwächungsrichtung ist deshalb situativ in 38/38 Fällen nützlich,
  nicht neutral. Die tatsächliche 2→1-Schreibung erfolgte jedoch erst nach der
  Handlung und änderte die nächste untersuchte Entscheidung in 0/38 Fällen.
- Die unmittelbare direkte Zielwirkung der ausgewählten Synapse ist in 38/38
  Fällen kausal nachgewiesen.
- Jeder Fall besitzt kontrolliert aktivierbare Alternativen. Einzel- und
  Kombinationsablationen zeigen in 38/38 Fällen eine fähigkeitsrelevante
  Alternative, die bei Ablation des ursprünglichen Pfads eine Aufgabenfunktion
  übernimmt. In der kritischen natürlichen Erfahrung änderte ihre
  Einzelablation die gemessene neuronale Antwort dagegen in 0/38 Fällen.
- Die ursprüngliche direkte Funktion kehrt in 38/38 Fällen zwischenzeitlich
  zurück: erstmals zwischen Erfahrung 10 und 47, insgesamt an 388
  diagnostizierten Schritten nach dem ersten Verlust. Es treten 356 erneute
  Verluste auf. Am Schritt 256 sind weiterhin 0/38 Pfade direkt funktional.
- Die W0/W1-Langzeitklassen bleiben unverändert: 16 nützlich, zehn schädlich,
  sieben neutral und fünf kontextabhängig. Sie beschreiben Unterschiede
  vollständiger Lerntrajektorien, nicht die Güte einer einzelnen Synapse.

## Umfang und Reproduktion

| Kennzahl | Ergebnis |
|---|---:|
| unabhängiger Sicherheits-Preflight | 12/12 |
| Regressionstests | 832/832 |
| B.5.11 / B.5.12 / B.5.13 Referenzdateien | 21 / 15 / 13 exakt |
| C.0 Referenzdateien | 14 exakt |
| historische C.0-Funktionsfälle | 19/19 |
| prospektive Läufe | 12.800 |
| registrierte neue Verbindungen | 166.880 |
| funktionale prospektive Fälle | 19 |
| Discovery / unabhängige Bestätigung | 7 / 12 |
| ausgewählte Fälle insgesamt | 38 |
| exakte Wiederholung aller korrigierten C.1-Shards | bestanden |
| Persistenz mit ausstehenden Ereignissen | bestanden |

## Vier getrennte Dimensionen

### A – Integrität

Der unabhängige Preflight bestand vor allen Lernexperimenten. Alle
Veränderungen waren nach Architekturentscheidung 0037 zulässig. Keine
Funktions- oder Erfolgsbeobachtung wurde zur Legitimierung eines
Integritätsverstoßes verwendet.

### B – Kausalität

H0, H1 und H2 starten aus demselben Zustand. H1 setzt nur die ausgewählte
Synapse vor Beginn der natürlichen Erfahrung diagnostisch auf null. Die
abweichende Handlung erhält ihre tatsächlich entstandene positive
Rückmeldung. H3 entfernt kombiniert die konkurrierenden Eingänge. Erfasst
werden Motorantwort, frühe und doppelte Aktivierung, Korrektheit, Konsequenz,
Zielaktivität sowie weitere Neuronen- und Synapsenaktivität.

Der Handlungsbeitrag ist eindeutig: Die ausgewählte Synapse verursacht in
38/38 Fällen den Übergang von der korrekten Einzelantwort zur falschen
Doppelantwort. Davon getrennt zeigt W0/W1, dass die spätere 2→1-Schreibung in
0/38 Fällen die nächste Entscheidung verändert. Die direkte Senderprobe zeigt
in 38/38 Fällen die unmittelbare Zielwirkung.

### C – Situative Angemessenheit

Die bisherige Einstufung 38 neutral wird korrigiert. Da die
Vor-Erfahrungs-Ablation den tatsächlichen Fehler in allen Fällen behebt, ist
die Abschwächungsrichtung 38-mal situativ nützlich. Das ist keine Behauptung,
dass die erst nach der Handlung erfolgende Schreibung rückwirkend geholfen
hätte; ihr Ein-Schritt-Folgeeffekt wurde nicht nachgewiesen.

### D – Langzeitwirkung

Die ursprünglichen Kategorien bleiben bei unveränderten Schwellenwerten
bestehen:

| Population | nützlich | schädlich | neutral | kontextabhängig |
|---|---:|---:|---:|---:|
| historisch | 7 | 4 | 3 | 5 |
| prospektiv | 9 | 6 | 4 | 0 |
| gesamt | 16 | 10 | 7 | 5 |

Jede Zeile nennt nun die geprüfte Fähigkeit (Abruf von Aufgabe A und B), den
kausalen Unterschied, den Maßstab (korrekte Erfahrungen und/oder finales
Repertoire), die nachgewiesene Rolle alternativer Pfade und die Abhängigkeit
von auseinanderlaufenden Lerntrajektorien. Alle 38 Vergleiche sind
trajektorienabhängig.

## Alternative Pfade und Wiederkehr

Pro ausgewähltem Pfad werden strukturelle, potenziell funktionale,
kontrolliert funktionale, neuronal kausale und fähigkeitskausale Alternativen
getrennt. In jedem Fall bestehen zwei kontrolliert aktivierbare Alternativen;
eine davon ist in der Abrufprobe fähigkeitskausal. Die gemeinsame Ablation von
ausgewähltem Pfad und tatsächlichen Alternativen senkt das Repertoire in
38/38 Fällen. Damit ist Aufgabenfunktionsübernahme nachgewiesen und der Begriff
funktionale Reorganisation für diesen eng definierten Abrufbefund zulässig.
Eine Beteiligung dieser Alternativen an der kritischen natürlichen Erfahrung
ist dagegen nicht nachgewiesen.

Die direkte Funktion wurde in diagnostischen Zustandskopien nach jedem Schritt
0–256 geprüft. Alle 38 Pfade verlieren und gewinnen Funktion mehrfach; kein
Fall ist am letzten Schritt funktional. Aussagen über kontinuierliche
Funktionslosigkeit wurden entfernt.

## Antworten auf die Abschlussfragen

1. **Handlungskausalität:** Ja, 38/38 Synapsen waren kausal an der falschen
   Doppelantwort beteiligt.
2. **Situative Abschwächung:** 38 nützlich, 0 schädlich, 0 neutral,
   0 unentschieden; die tatsächliche Schreibung lag zeitlich nach der Handlung.
3. **Funktionale Alternativen:** Ja, kontrollierte Zielaktivierung in 38/38
   Fällen.
4. **Aufgabenübernahme:** Ja, durch Einzel- und Kombinationsablation in 38/38
   Fällen für den kontrollierten Fähigkeitsabruf.
5. **Zwischenzeitliche Wiederkehr:** Ja, 38/38, erstmals bei Schritt 10–47;
   danach wiederholte Verluste, 0/38 bei Schritt 256.
6. **Langzeitklassen:** Ja, 16/10/7/5 bleiben gültig, aber nur als
   trajektorienabhängige W0/W1-Vergleiche.
7. **Für C.2 abgesichert:** Eligibility und lokale Modulation erklären die
   Schreibung; Handlungsbeitrag, spätere Schreibwirkung, direkte Zielwirkung,
   Fähigkeitsübernahme und zeitliche Wiederkehr müssen getrennt bleiben.

## Merge- und Produktionsentscheidung

**C.1 nach `main` mergen: Ja – für Forschungsinfrastruktur, korrigierte
Methodik und wissenschaftliche Befunde.**

**Produktionsfreigabe: Nein.** Es wird keine neue Lernregel, kein pauschaler
Gewichtsschutz, kein Einfrieren, kein Rollback und keine Aktivierung von
Growth, interner Exploration oder S4-d16 freigegeben.
