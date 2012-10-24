#include "Model3D.h"


Model3D::Model3D()
{
    this->angle[0] = this->angle[1] = this->angle[2] = 0.0f;
    this->scaleFactor = 1.0f;
}

Model3D::Model3D(string filename)
{
    this->angle[0] = this->angle[1] = this->angle[2] = 0.0f;
    this->scaleFactor = 1.0f;

    this->load(filename);
}

Model3D::~Model3D()
{
    //dtor
}

void Model3D::load(string filename)
{
    this->vertex.clear();
    this->normal.clear();
    this->face.clear();

    Vector3 vtemp;
    string line;
    string format = "%d//%d";
    int vi, vin;
    Face facetemp;

    ifstream in(filename.c_str());

    while( getline(in, line) )
    {
        if( line[0] == '#' ) continue;

        if( line[0] == 'v' && line[1] == 'n' )
        {
            sscanf(line.substr(2).c_str(), "%f %f %f", &vtemp[0], &vtemp[1], &vtemp[2]);

            this->normal.push_back(vtemp);
        }
        else if( line[0] == 'v')
        {
            sscanf(line.substr(1).c_str(), "%f %f %f", &vtemp[0], &vtemp[1], &vtemp[2]);
            this->vertex.push_back(vtemp);
        }
        else if( line[0] == 'f' )
        {
            facetemp.v.clear();
            facetemp.vn.clear();
            format = "%d//%d";

            while( sscanf(line.substr(1).c_str(), format.c_str(), &vi, &vin) == 2 )
            {
                vi--;
                vin--;
                facetemp.v.push_back(vi);
                facetemp.vn.push_back(vin);

                format = "%*d//%*d " + format;
            }

            this->face.push_back(facetemp);
        }

    }


    in.close();
}
