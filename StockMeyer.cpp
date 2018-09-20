#include <iostream>
#include <vector>
#include <stack>

using namespace std;

bool isOperator(char o) {
    if (o == 'V' || o == 'H') { return true; }
    return false;
}

bool isVertical(char o) {
    if (o == 'V') { return true;}
    return false;
}

int psi(int w, int h) {
    return w*h;
}

struct Dimension {
    int height;
    int width;
    vector<char> ids;
    vector<int> orientationIDs;
    vector<Dimension> orients;

    vector<Dimension> constructOrientations (int h, int w) {
        vector<Dimension> orients;
        Dimension one = Dimension(h, w);
        one.orientationIDs.push_back(0);
        orients.push_back(one);
        if (h != w) {
            Dimension two = Dimension(w, h);
            two.orientationIDs.push_back(1);
            orients.push_back(two);
        }
        return orients;
    }

    void giveToEachOrient(char id) {
        for (int i = 0; i < orients.size(); i++) {
            orients[i].ids.push_back(id);
        }
    }

    Dimension(int h, int w) {
        height = h;
        width = w;
    }
    void setOrients() {
        orients = constructOrientations(height, width);
    }
    Dimension() {}
};

bool myComparator(Dimension i, Dimension j) {
    if (i.height == j.height) { return i.width < j.width; }
    else return i.height > j.height;
}

vector<Dimension> sortList(vector<Dimension> o) {
    sort(o.begin(), o.end(), myComparator);
    return o;
}

Dimension horizontalSlice(Dimension one, Dimension two) {
    Dimension newdim;
    newdim.height = one.height + two.height;
    newdim.width = max(one.width, two.width);
    newdim.orientationIDs = one.orientationIDs;
    newdim.orientationIDs.insert(newdim.orientationIDs.end(), two.orientationIDs.begin(), two.orientationIDs.end());
    newdim.ids = one.ids;
    newdim.ids.insert(newdim.ids.end(), two.ids.begin(), two.ids.end());
    return newdim;
}


Dimension verticalSlice(Dimension one, Dimension two) {
    Dimension newdim;
    newdim.height = max(one.height, two.height);
    newdim.width = one.width + two.width;
    newdim.orientationIDs = one.orientationIDs;
    newdim.orientationIDs.insert(newdim.orientationIDs.end(), two.orientationIDs.begin(), two.orientationIDs.end());
    newdim.ids = one.ids;
    newdim.ids.insert(newdim.ids.end(), two.ids.begin(), two.ids.end());
    return newdim;
}

Dimension verticalMerge(Dimension one, Dimension two) {
    int i = 0;
    int j = 0;
    Dimension newDim;
    while (i < one.orients.size() && j < two.orients.size()) {
        newDim.orients.push_back(verticalSlice(one.orients[i], two.orients[j]));
        if (one.orients[i].height > two.orients[j].height) { i++; }
        else if (one.orients[i].height < two.orients[j].height) { j++; }
        else if (one.orients[i].height == two.orients[j].height) { i++; j++; }
    }
    newDim.orients = sortList(newDim.orients);
    return newDim;
}

Dimension horizontalMerge(Dimension one, Dimension two) {
    int i = 0;
    int j = 0;
    Dimension newDim;
    while (i < one.orients.size() && j < two.orients.size()) {
        newDim.orients.push_back(horizontalSlice(one.orients[i], two.orients[j]));
        if (one.orients[i].width > two.orients[j].width) { j++; }
        else if (one.orients[i].width < two.orients[j].width) { i++; }
        else if (one.orients[i].width == two.orients[j].width) { i++; j++; }
    }
    newDim.orients = sortList(newDim.orients);
    return newDim;
}

Dimension findBestDim(Dimension d) {
    cout << "+++++++++++++++++++++" << endl;
    Dimension minD;
    int mini = INT_MAX;
    for (int i = 0; i < d.orients.size(); i++) {
        if (psi(d.orients[i].width, d.orients[i].height) < mini) {
            mini = psi(d.orients[i].width, d.orients[i].height);
            minD = d.orients[i];
        }
    }
    for (int i = 0; i < minD.orientationIDs.size(); i++) {
        cout << "ID of the rectangle: " << minD.ids[i] << endl;
        cout << "Orientation: " << minD.orientationIDs[i] << endl;
        cout << "    ----    " << endl;
    }
    cout << "Dimensions of the enveloping rectangle are " << endl;
    cout << "Height: " << minD.height << " Width: " << minD.width << endl;
    cout << "====================END==================" << endl;

    return minD;
}

Dimension PEtoEnvelope(vector<char> pe, vector<Dimension> dimensions) {
    stack<Dimension> thestack;
    int dimCounter = 0;
    Dimension newDim;
    for (int i = 0; i < pe.size(); i++) {
        if (!isOperator(pe[i])) {
            newDim = dimensions[dimCounter];
            newDim.giveToEachOrient(pe[i]);
            thestack.push(newDim);
            dimCounter++;
        } else {
                Dimension one = thestack.top();
                one.orients = sortList(one.orients);
                thestack.pop();
                Dimension two = thestack.top();
                two.orients = sortList(two.orients);
                thestack.pop();
            if(isVertical(pe[i])) {
                newDim = verticalMerge(one, two);
            } else {
                newDim = horizontalMerge(one, two);
            }
            thestack.push(newDim);   
        }
    }
    return newDim;
}

int main(int argc, char* argv[]) {

    string PELong = "4512H3VHV";
    vector<char> PE(PELong.begin(), PELong.end());
    vector<Dimension> dimensions = { Dimension(5, 2), Dimension(2, 3), Dimension(2, 1), Dimension(1, 1), Dimension(2, 3) };
    for (int i = 0; i < dimensions.size(); i++) {
        dimensions[i].setOrients();
    }
    Dimension d = PEtoEnvelope(PE, dimensions);
    findBestDim(d);
}

