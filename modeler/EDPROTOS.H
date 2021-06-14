/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Program  ED - A DATABASE MODELING TOOL                     บ
บ                                                                         บ
บ              EDPROTOS.H Function Prototypes for erveryone               บ
บ                                                                         บ
บ              written by William A. Tolhurst                             บ
บ                                                                         บ
บ       This software is NOT Copyrighted.  Please feel free to use it     บ
บ       in any way you see fit, either in its entirety or in whatever     บ
บ       bits and pieces you need.  I, however, will not assume any re-    บ
บ       sponsibility for any aspect of its use, and do not claim suit-    บ
บ       ability for a particular purpose.  Enjoy!                         บ
บ                                                                         บ
บ       NOTE:  This statement does not affect the copyright status of     บ
บ              other software that accompanies it.  Please respect the    บ
บ              Copyright claims exercised by the other software authors.  บ
บ                                                                         บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

// EDCMD.C
extern int Get_Cmd (int *);

// EDDIA.H
extern void Clear_Dialog (void);
extern void Put_Dialog (int, int, char *);
extern char * Get_Dialog (int, int, char *);

// EDEDIT.H
extern void Edit_Vertex (TDA, int);
extern void Edit_Polygon (struct PrimType *);
extern void Edit_Sphere (struct PrimType *);
extern void Edit_Cone (struct PrimType *);
extern void Edit_Patch (struct PrimType *);
extern void Edit_Ring (struct PrimType *);
extern void Edit_Primitive (struct PrimType *);
extern void Edit_Object (struct ObjectType *);
extern void Edit_Studio (void);
extern void Edit_SysParms (void);

// EDFILE.C
extern void Write_Vector (FILE *, char *, TDA);
extern void Write_Color (FILE *, char *, struct ColorType);
extern void Write_Polygon (struct PrimType *, FILE *);
extern void Write_Cone (struct PrimType *, FILE *);
extern void Write_Sphere (struct PrimType *, FILE *);
extern void Write_Patch (struct PrimType *, FILE *);
extern void Write_Ring (struct PrimType *, FILE *);
extern void Write_Surface (struct ObjectType *, FILE *);
extern void Write_Transform (struct ObjectType *, FILE *);
extern void Write_Object (struct ObjectType *, FILE *);
extern void Write_Studio (FILE *);
extern void Write_Session (void);
extern void Read_Polygon (struct PrimType *, FILE *);
extern void Read_Cone (struct PrimType *, FILE *);
extern void Read_Sphere (struct PrimType *, FILE *);
extern void Read_Patch (struct PrimType *, FILE *);
extern void Read_Ring (struct PrimType *, FILE *);
extern int Read_Primitives (struct ObjectType *, FILE *);
extern void Read_Object (void);
extern void Read_Surface (FILE *);
extern void Read_Transform (FILE *);
extern void Read_Studio (FILE *);
extern void Read_Session (void);

// EDGRAPH.C
extern void Update_ESR_Matrix (void);
extern void WorldSpace_To_EyeSpace (TDA, TDA);
extern void Update_ViewPlane_Dist (void);
extern void Initialize_ScreenSpace_Values (void);
extern int Hither_Clip_Line (TDA, TDA);
extern void EyeSpace_To_ScreenSpace (TDA, float *, float *);
extern int Clip_Point (float, float);
extern int Clip_To_Edge ( float, float, float *, float *);
extern int Screen_Clip_Line (float *, float *, float *, float *);
extern void Render_Line (TDA, TDA, int);
extern void Transform_Point (struct TransformType *, TDA, TDA);
extern void Draw_Polygon(struct PrimType *, struct TransformType *);
extern void Draw_Cone (struct PrimType *, struct TransformType *);
extern void Draw_Sphere (struct PrimType *, struct TransformType *);
extern void Draw_Patch (struct PrimType *, struct TransformType *);
extern void Draw_Ring (struct PrimType *, struct TransformType *);
extern void Draw_Primitive (struct PrimType *, struct TransformType *);
extern void Draw_Object(struct ObjectType *);
extern void HighLight_Primitive(void);
extern void UnHighLight_Primitive(void);
extern void HighLight_Object (void);
extern void UnHighLight_Object (void);
extern void Draw_View (void);
extern void Initialize_Graphics (void);
extern void SubDivide_Screen (void);
extern void Shutdown_Graphics (void);

// EDMAIN.C
extern int main (void);

// EDMOVE.C
extern void Move_Viewer (int);

// EDOBJECT.H
extern struct ObjectType * Insert_Object (char *);
extern void Add_Object (void);
extern void Select_Object (void);
extern void Deallocate_Object (struct ObjectType *);
extern void Delete_Object (struct ObjectType *);
extern void Scale_Object (int);
extern void Move_Object (int);

// EDOCMD.C
extern int Get_Object_Cmd (int *);

// EDOEXEC.C
extern void Object_Exec (void);

// EDOMOVE.C
extern void Move_Object (int);

// EDPRIM.C
extern struct PrimType * Insert_Primitive (struct ObjectType *);
extern int Determine_Prim_Index (char *);
extern void Add_Primitive (struct ObjectType *);
extern void Deallocate_Primitive (struct PrimType *);
extern void Delete_Primitive (void);
extern void Select_Primitive (void);

//EDSTAT.C
extern void Put_FStatus (int , int , char *, float);
extern void Put_IStatus (int , int , char *, int);
extern void Put_SStatus (int , int , char *);
extern void Display_Studio_Parms (void);
extern void Display_Status (void);

// EDSTCMD.C
extern int Get_Studio_Cmd (void);

// EDSTEXEC.C
extern void Studio_Exec (void);

// EDSYS.C
extern void Initialize_Viewer (void);
extern void Initialize_Studio (void);
extern void Initialize_Object_Data (void);
extern void Initialize_System_Parms (void);
extern void Delete_All_Objects (void);
extern void Initialize_System (void);
extern void Shutdown_System (void);
extern void Error_Beep (void);

// EDVCMD.C
extern int Get_Vertex_Cmd (void);

// EDVERTEX.C
extern struct VertexType * Insert_Vertex (struct PrimType *);
extern void Add_Vertex (void);
extern void Select_Vertex (void);
extern void Delete_Vertex (struct PrimType *, struct VertexType *);

// EDVEXEC.C
extern void Vertex_Exec (void);

