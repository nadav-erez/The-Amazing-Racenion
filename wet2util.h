// 234218 Data Structures 1.
// Semester: 2026 B (Spring).
// Wet Exercise #2.
//
// Skill class works over Z/SKILL_MOD Z (modular arithmetic).
// 

#ifndef WET2_UTIL_H_
#define WET2_UTIL_H_

#include <ostream>

enum struct StatusType {
    SUCCESS          = 0,
    ALLOCATION_ERROR = 1,
    INVALID_INPUT    = 2,
    FAILURE          = 3,
};


template<typename T>
class output_t {
private:
    const StatusType __status;
    const T __ans;

public:
    output_t() : __status(StatusType::SUCCESS), __ans(T()) { }
    output_t(StatusType status) : __status(status), __ans(T()) { }
    output_t(const T &ans) : __status(StatusType::SUCCESS), __ans(ans) { }

    StatusType status() { return __status; }
    T ans() { return __ans; }
};


// ── Modular arithmetic helpers ───────────────────────────────────────────────

// Largest prime p where 2*(p-1)^2 < INT_MAX, so every computation
// (including sums of two matrix products) fits in a plain int.
static const int SKILL_MOD = 32749;
static const int EPS = 0;

static int _sm(int x) {
    return ((x % SKILL_MOD) + SKILL_MOD) % SKILL_MOD;
}

static int _spow(int base, int exp) {
    int result = 1;
    base = _sm(base);
    while (exp > 0) {
        if (exp & 1) result = result * base % SKILL_MOD;
        base = base * base % SKILL_MOD;
        exp >>= 1;
    }
    return result;
}

// Modular inverse via Fermat's little theorem (SKILL_MOD must be prime)
static int _sinv(int a) {
    return _spow(_sm(a), SKILL_MOD - 2);
}


// ── Skill class (modular) ────────────────────────────────────────────────────

class Skill {
private:
    int m[2][2];
    bool m_valid;

public:

    Skill() : m_valid(true) {
        m[0][0] = 1; m[0][1] = 0;
        m[1][0] = 0; m[1][1] = 1;
    }

    virtual ~Skill() = default;

    Skill(int a, int b, int c, int d) : m_valid(true) {
        m[0][0] = _sm(a);
        m[0][1] = _sm(b);
        m[1][0] = _sm(c);
        m[1][1] = _sm(d);
        if (det() == 0) {
            m_valid = false;
        }
    }

    Skill(const Skill &other) : m_valid(other.m_valid) {
        m[0][0] = other.m[0][0]; m[0][1] = other.m[0][1];
        m[1][0] = other.m[1][0]; m[1][1] = other.m[1][1];
    }

    // Assignment operator
    Skill& operator=(const Skill &other) {
        if (this != &other) {
            m_valid = other.m_valid;
            m[0][0] = other.m[0][0]; m[0][1] = other.m[0][1];
            m[1][0] = other.m[1][0]; m[1][1] = other.m[1][1];
        }
        return *this;
    }

    static Skill identity() { return Skill(); }

    static Skill invalid() {
        Skill s(0, 0, 0, 0);
        s.m_valid = false;
        return s;
    }

    // All products and sums of products fit in int for SKILL_MOD=32749.
    // matrix determinant (ad - bc) 
    int det() const {
        return _sm(m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    }

    // check if the skill is valid (e.g for the print operator <<)
    bool isValid() const {
        return m_valid && (det() != 0);
    }

    int getEffectiveSkill() const {
        return det();
    }

    // Modular matrix inverse
    Skill inv() const {
        int d = det();
        if (d == 0) return Skill::invalid();
        int dinv = _sinv(d);
        return Skill(
            _sm( m[1][1] * dinv),
            _sm(-m[0][1] * dinv),
            _sm(-m[1][0] * dinv),
            _sm( m[0][0] * dinv)
        );
    }

    // check if two skills are equal (modular)
    bool operator==(const Skill &other) const {
        return m[0][0] == other.m[0][0] &&
               m[0][1] == other.m[0][1] &&
               m[1][0] == other.m[1][0] &&
               m[1][1] == other.m[1][1];
    }

    bool operator<(const Skill &other) const {
        return det() < other.det();
    }

    bool operator>(const Skill &other) const {
        return det() > other.det();
    }

    Skill& operator*=(const Skill &other) {
        int a = m[0][0], b = m[0][1];
        int c = m[1][0], d = m[1][1];
        int a2 = other.m[0][0], b2 = other.m[0][1];
        int c2 = other.m[1][0], d2 = other.m[1][1];

        m[0][0] = _sm(a*a2 + b*c2);
        m[0][1] = _sm(a*b2 + b*d2);
        m[1][0] = _sm(c*a2 + d*c2);
        m[1][1] = _sm(c*b2 + d*d2);

        if (det() == 0) {
            m_valid = false;
        }
        return *this;
    }

    
    friend Skill operator*(Skill a, const Skill &b) {
        a *= b;
        return a;
    }

    friend std::ostream& operator<<(std::ostream &out, const Skill &obj) {
        if (!obj.isValid()) {
            out << "Invalid Skill";
            return out;
        }
        out << "[" << obj.m[0][0] << "," << obj.m[0][1] << ";"
            << obj.m[1][0] << "," << obj.m[1][1] << "]";
        return out;
    }
};



#endif // WET2_UTIL_H_
