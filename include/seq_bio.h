#ifndef seq_bio_h
#define seq_bio_h

#include <string>
#include <vector>

class ses_seq { 
    // si on utise pas de mot clé tout est privé 
    private:

    std::string _description;
    std::string _data;
    std::string _quality;

    
    //protected:   c'est mot clé meme les programme vosins ont acces 

    public:
    //seq_bio();
    // seq_bio(std::string d , std::string s );
    // // seq_bio(std::string d , std::string s );
      // // seq_bio(std::string d , std::string s , std::string q );

    enum Type {unknown, dna, rna, amino_acide}; //ic uknows va avoir une valeur 0


  
    private:

    std::string getDescription;
    std::string getSequenece;

    double getQuality(size_t p); // obtenir la quality d'un caractere precis de la position p : size_t entier no signé le plus grand possible
    void setQuality(std::string s);
    std:: string setQuality();

    void setType(Type T);


};

#endif