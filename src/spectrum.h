#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

class Spectrum {
public:
    Spectrum(const std::string& sequence, int k);
    int size() const;
    
private:
    std::vector<uint64_t> unique;
    std::vector<int> counts;
};

#endif // _SPECTRUM_H_