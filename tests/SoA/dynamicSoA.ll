; ModuleID = 'dynamic_SoA.c'
source_filename = "dynamic_SoA.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.StructureOne = type { ptr, ptr, ptr }

@.str = private unnamed_addr constant [11 x i8] c"Array a: \0A\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"Index %d: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"Array b: \0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"Array c: \0A\00", align 1
@.str.4 = private unnamed_addr constant [14 x i8] c"Index %d: %c\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateStructure(ptr noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  store i32 0, ptr %9, align 4
  br label %10

10:                                               ; preds = %22, %4
  %11 = load i32, ptr %9, align 4
  %12 = load i32, ptr %6, align 4
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %25

14:                                               ; preds = %10
  %15 = load i32, ptr %9, align 4
  %16 = load ptr, ptr %5, align 8
  %17 = getelementptr inbounds %struct.StructureOne, ptr %16, i32 0, i32 0
  %18 = load ptr, ptr %17, align 8
  %19 = load i32, ptr %9, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds i32, ptr %18, i64 %20
  store i32 %15, ptr %21, align 4
  br label %22

22:                                               ; preds = %14
  %23 = load i32, ptr %9, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr %9, align 4
  br label %10, !llvm.loop !6

25:                                               ; preds = %10
  store i32 0, ptr %9, align 4
  br label %26

26:                                               ; preds = %38, %25
  %27 = load i32, ptr %9, align 4
  %28 = load i32, ptr %7, align 4
  %29 = icmp slt i32 %27, %28
  br i1 %29, label %30, label %41

30:                                               ; preds = %26
  %31 = load i32, ptr %9, align 4
  %32 = load ptr, ptr %5, align 8
  %33 = getelementptr inbounds %struct.StructureOne, ptr %32, i32 0, i32 1
  %34 = load ptr, ptr %33, align 8
  %35 = load i32, ptr %9, align 4
  %36 = sext i32 %35 to i64
  %37 = getelementptr inbounds i32, ptr %34, i64 %36
  store i32 %31, ptr %37, align 4
  br label %38

38:                                               ; preds = %30
  %39 = load i32, ptr %9, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr %9, align 4
  br label %26, !llvm.loop !8

41:                                               ; preds = %26
  store i32 0, ptr %9, align 4
  br label %42

42:                                               ; preds = %55, %41
  %43 = load i32, ptr %9, align 4
  %44 = load i32, ptr %8, align 4
  %45 = icmp slt i32 %43, %44
  br i1 %45, label %46, label %58

46:                                               ; preds = %42
  %47 = load i32, ptr %9, align 4
  %48 = trunc i32 %47 to i8
  %49 = load ptr, ptr %5, align 8
  %50 = getelementptr inbounds %struct.StructureOne, ptr %49, i32 0, i32 2
  %51 = load ptr, ptr %50, align 8
  %52 = load i32, ptr %9, align 4
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds i8, ptr %51, i64 %53
  store i8 %48, ptr %54, align 1
  br label %55

55:                                               ; preds = %46
  %56 = load i32, ptr %9, align 4
  %57 = add nsw i32 %56, 1
  store i32 %57, ptr %9, align 4
  br label %42, !llvm.loop !9

58:                                               ; preds = %42
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @createStructureOne() #0 {
  %1 = alloca ptr, align 8
  %2 = call noalias ptr @malloc(i64 noundef 24) #4
  store ptr %2, ptr %1, align 8
  %3 = call noalias ptr @malloc(i64 noundef 400000) #4
  %4 = load ptr, ptr %1, align 8
  %5 = getelementptr inbounds %struct.StructureOne, ptr %4, i32 0, i32 0
  store ptr %3, ptr %5, align 8
  %6 = call noalias ptr @malloc(i64 noundef 800000) #4
  %7 = load ptr, ptr %1, align 8
  %8 = getelementptr inbounds %struct.StructureOne, ptr %7, i32 0, i32 1
  store ptr %6, ptr %8, align 8
  %9 = call noalias ptr @malloc(i64 noundef 300000) #4
  %10 = load ptr, ptr %1, align 8
  %11 = getelementptr inbounds %struct.StructureOne, ptr %10, i32 0, i32 2
  store ptr %9, ptr %11, align 8
  %12 = load ptr, ptr %1, align 8
  ret ptr %12
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @freeStructure(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %struct.StructureOne, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  call void @free(ptr noundef %5) #5
  %6 = load ptr, ptr %2, align 8
  %7 = getelementptr inbounds %struct.StructureOne, ptr %6, i32 0, i32 1
  %8 = load ptr, ptr %7, align 8
  call void @free(ptr noundef %8) #5
  %9 = load ptr, ptr %2, align 8
  %10 = getelementptr inbounds %struct.StructureOne, ptr %9, i32 0, i32 2
  %11 = load ptr, ptr %10, align 8
  call void @free(ptr noundef %11) #5
  %12 = load ptr, ptr %2, align 8
  call void @free(ptr noundef %12) #5
  ret void
}

; Function Attrs: nounwind
declare void @free(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printStructure(ptr noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  store i32 0, ptr %9, align 4
  br label %11

11:                                               ; preds = %25, %4
  %12 = load i32, ptr %9, align 4
  %13 = load i32, ptr %6, align 4
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %15, label %28

15:                                               ; preds = %11
  %16 = load i32, ptr %9, align 4
  %17 = load ptr, ptr %5, align 8
  %18 = getelementptr inbounds %struct.StructureOne, ptr %17, i32 0, i32 0
  %19 = load ptr, ptr %18, align 8
  %20 = load i32, ptr %9, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds i32, ptr %19, i64 %21
  %23 = load i32, ptr %22, align 4
  %24 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %16, i32 noundef %23)
  br label %25

25:                                               ; preds = %15
  %26 = load i32, ptr %9, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, ptr %9, align 4
  br label %11, !llvm.loop !10

28:                                               ; preds = %11
  %29 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  store i32 0, ptr %9, align 4
  br label %30

30:                                               ; preds = %44, %28
  %31 = load i32, ptr %9, align 4
  %32 = load i32, ptr %7, align 4
  %33 = icmp slt i32 %31, %32
  br i1 %33, label %34, label %47

34:                                               ; preds = %30
  %35 = load i32, ptr %9, align 4
  %36 = load ptr, ptr %5, align 8
  %37 = getelementptr inbounds %struct.StructureOne, ptr %36, i32 0, i32 1
  %38 = load ptr, ptr %37, align 8
  %39 = load i32, ptr %9, align 4
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds i32, ptr %38, i64 %40
  %42 = load i32, ptr %41, align 4
  %43 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %35, i32 noundef %42)
  br label %44

44:                                               ; preds = %34
  %45 = load i32, ptr %9, align 4
  %46 = add nsw i32 %45, 1
  store i32 %46, ptr %9, align 4
  br label %30, !llvm.loop !11

47:                                               ; preds = %30
  %48 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  store i32 0, ptr %9, align 4
  br label %49

49:                                               ; preds = %64, %47
  %50 = load i32, ptr %9, align 4
  %51 = load i32, ptr %8, align 4
  %52 = icmp slt i32 %50, %51
  br i1 %52, label %53, label %67

53:                                               ; preds = %49
  %54 = load i32, ptr %9, align 4
  %55 = load ptr, ptr %5, align 8
  %56 = getelementptr inbounds %struct.StructureOne, ptr %55, i32 0, i32 2
  %57 = load ptr, ptr %56, align 8
  %58 = load i32, ptr %9, align 4
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds i8, ptr %57, i64 %59
  %61 = load i8, ptr %60, align 1
  %62 = sext i8 %61 to i32
  %63 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %54, i32 noundef %62)
  br label %64

64:                                               ; preds = %53
  %65 = load i32, ptr %9, align 4
  %66 = add nsw i32 %65, 1
  store i32 %66, ptr %9, align 4
  br label %49, !llvm.loop !12

67:                                               ; preds = %49
  ret void
}

declare i32 @printf(ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
  %3 = call ptr @createStructureOne()
  store ptr %3, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  call void @freeStructure(ptr noundef %4)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
