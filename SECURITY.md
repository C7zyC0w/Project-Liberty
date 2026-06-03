# Security Policy

LibertyOS takes security seriously. We appreciate the efforts of security researchers and the community in helping identify and responsibly disclose vulnerabilities.

---

## Supported Versions

LibertyOS is currently in pre-release development (0.x.x). Dedicated security features are not yet implemented — this work is planned for the 1.x.x release series onwards. As such, **no 0.x.x version is supported for production or security-sensitive use**.

| Version | Supported                          |
| ------- | ---------------------------------- |
| 0.1     | ✘ No longer supported              |
| 0.2     | ✘ No longer supported              |
| 0.3     | ✘ No longer supported              |
| 0.4     | ✘ No longer supported              |
| 0.5.x     | ✘ Current pre-release — no security support |
| 1.x.x   | ✔ Planned — security support begins here |

The jump from 0.5 to 1.x.x reflects the point at which security infrastructure will be introduced. Once 1.0.0 is released, all 0.x.x versions will be formally end-of-life with no further patches of any kind.

> **Note:** All versions prior to 1.0.0 are pre-release and have **no published release files or build artifacts**. LibertyOS is not yet ready for general use or distribution. Version 1.0.0 will be the first official release, at which point binaries and release packages will be made available.

---

## Reporting a Vulnerability

**Please do not report security vulnerabilities through public GitHub issues.**

If you believe you have found a security vulnerability in LibertyOS, please report it responsibly by opening a [GitHub Security Advisory](https://github.com/your-org/LibertyOS/security/advisories/new) on this repository. This keeps the report private until a fix is ready.

Alternatively, you may contact the maintainers directly via the email listed on the repository owner's GitHub profile.

### What to include

To help us triage and reproduce the issue quickly, please provide as much of the following as possible:

- A clear description of the vulnerability and its potential impact
- The component or subsystem affected (e.g. kernel, bootloader, networking stack)
- Steps to reproduce or a minimal proof-of-concept
- The version or commit hash you tested against
- Any suggested mitigations, if you have them

---

## Response Timeline

We aim to follow these response windows after a report is received:

| Milestone                        | Target timeframe |
| -------------------------------- | ---------------- |
| Acknowledgement of report        | Within 72 hours  |
| Initial triage and severity assessment | Within 7 days |
| Fix developed (for confirmed issues) | Within 30 days |
| Public disclosure (after fix)    | Coordinated with reporter |

These are targets, not guarantees — LibertyOS is a small open-source project and timelines may vary. We will keep you updated throughout the process.

---

## Disclosure Policy

We follow a **coordinated disclosure** model:

1. You report the issue privately to the maintainers.
2. We confirm receipt, investigate, and develop a fix.
3. A patched release is prepared and staged.
4. We coordinate a public disclosure date with you — typically 90 days from initial report, or sooner if a fix is ready.
5. A security advisory is published alongside the patched release, crediting you if you wish.

We ask that you do **not** publicly disclose the vulnerability until a fix has been released or the 90-day window has passed, whichever comes first.

---

## Scope

The following are considered in scope for security reports:

- The LibertyOS kernel and core system components
- The bootloader and hardware abstraction layer
- Default system utilities and daemons shipped with LibertyOS
- Build toolchain and release artifacts (e.g. supply chain concerns)

The following are generally **out of scope**:

- Third-party software packaged for LibertyOS but not maintained by this project
- Vulnerabilities in the host hardware or firmware
- Issues present only in unsupported (0.x.x) versions
- Theoretical vulnerabilities with no demonstrated impact

---

## Recognition

We genuinely value the work of security researchers. If you responsibly disclose a valid vulnerability, we will:

- Credit you by name (or handle) in the security advisory, if you wish
- Acknowledge your contribution in the relevant release notes

We do not currently offer a bug bounty programme, but this may change as the project matures.

---

*This policy was last updated June 2026.*
