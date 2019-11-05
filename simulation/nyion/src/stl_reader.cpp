// thanks to Devert Alexandre
// def is_inside_naive(triangles, X):
// 	# Compute triangle vertices and their norms relative to X
// 	M = triangles - X
// 	M_norm = numpy.sqrt(numpy.sum(M ** 2, axis = 2))
//
// 	# Accumulate generalized winding number per triangle
// 	winding_number = 0.
// 	for (A, B, C), (a, b, c) in zip(M, M_norm):
// 		winding_number += numpy.arctan2(numpy.linalg.det(numpy.array([A, B, C])), (a * b * c) + c * numpy.dot(A, B) + a * numpy.dot(B, C) + b * numpy.dot(C, A))
//
// 	# Job done
// 	return winding_number >= 2. * numpy.pi

//thanks to http://www.sgh1.net/posts/read-stl-file.md
// void read_stl(string fname, vector <tri>&v){
//
//     //!!
//     //don't forget ios::binary
//     //!!
//     ifstream myFile (
//         fname.c_str(), ios::in | ios::binary);
//
//     char header_info[80] = "";
//     char nTri[4];
//     unsigned long nTriLong;
//
//     //read 80 byte header
//     if (myFile) {
//         myFile.read (header_info, 80);
//         cout <<"header: " << header_info << endl;
//     }
//     else{
//         cout << "error" << endl;
//     }
//
//     //read 4-byte ulong
//     if (myFile) {
//         myFile.read (nTri, 4);
//         nTriLong = *((unsigned long*)nTri) ;
//         cout <<"n Tri: " << nTriLong << endl;
//     }
//     else{
//         cout << "error" << endl;
//     }
//
//     //now read in all the triangles
//     for(int i = 0; i < nTriLong; i++){
//
//         char facet[50];
//
//         if (myFile) {
//
//         //read one 50-byte triangle
//             myFile.read (facet, 50);
//
//         //populate each point of the triangle
//         //using v3::v3(char* bin);
//             //facet + 12 skips the triangle's unit normal
//             v3 p1(facet+12);
//             v3 p2(facet+24);
//             v3 p3(facet+36);
//
//             //add a new triangle to the array
//             v.push_back( tri(p1,p2,p3) );
//
//         }
//     }
//
//     return;
//
// }
