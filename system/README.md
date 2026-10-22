# System layer (SI-P-05 Platform services, Class C)

Shared services: ThreadX wrapper (osal/), sensing, storage, journal, power, diagnostics,
communications, inter-MCU protocol, service port and watchdog health aggregation.
May include driver/ headers. Never includes application/ headers; higher layers are
notified through callbacks declared in system/ headers.
journal/ is the only writer of journal records; readers use journal/api/.
