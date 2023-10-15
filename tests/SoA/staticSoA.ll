; ModuleID = 'static_SoA.c'
source_filename = "static_SoA.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.StructureOne = type { [100000 x i32], [200000 x i32], [300000 x i8] }

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

10:                                               ; preds = %21, %4
  %11 = load i32, ptr %9, align 4
  %12 = load i32, ptr %6, align 4
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %24

14:                                               ; preds = %10
  %15 = load i32, ptr %9, align 4
  %16 = load ptr, ptr %5, align 8
  %17 = getelementptr inbounds %struct.StructureOne, ptr %16, i32 0, i32 0
  %18 = load i32, ptr %9, align 4
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds [100000 x i32], ptr %17, i64 0, i64 %19
  store i32 %15, ptr %20, align 4
  br label %21

21:                                               ; preds = %14
  %22 = load i32, ptr %9, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %9, align 4
  br label %10, !llvm.loop !6

24:                                               ; preds = %10
  store i32 0, ptr %9, align 4
  br label %25

25:                                               ; preds = %36, %24
  %26 = load i32, ptr %9, align 4
  %27 = load i32, ptr %7, align 4
  %28 = icmp slt i32 %26, %27
  br i1 %28, label %29, label %39

29:                                               ; preds = %25
  %30 = load i32, ptr %9, align 4
  %31 = load ptr, ptr %5, align 8
  %32 = getelementptr inbounds %struct.StructureOne, ptr %31, i32 0, i32 1
  %33 = load i32, ptr %9, align 4
  %34 = sext i32 %33 to i64
  %35 = getelementptr inbounds [200000 x i32], ptr %32, i64 0, i64 %34
  store i32 %30, ptr %35, align 4
  br label %36

36:                                               ; preds = %29
  %37 = load i32, ptr %9, align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, ptr %9, align 4
  br label %25, !llvm.loop !8

39:                                               ; preds = %25
  store i32 0, ptr %9, align 4
  br label %40

40:                                               ; preds = %52, %39
  %41 = load i32, ptr %9, align 4
  %42 = load i32, ptr %8, align 4
  %43 = icmp slt i32 %41, %42
  br i1 %43, label %44, label %55

44:                                               ; preds = %40
  %45 = load i32, ptr %9, align 4
  %46 = trunc i32 %45 to i8
  %47 = load ptr, ptr %5, align 8
  %48 = getelementptr inbounds %struct.StructureOne, ptr %47, i32 0, i32 2
  %49 = load i32, ptr %9, align 4
  %50 = sext i32 %49 to i64
  %51 = getelementptr inbounds [300000 x i8], ptr %48, i64 0, i64 %50
  store i8 %46, ptr %51, align 1
  br label %52

52:                                               ; preds = %44
  %53 = load i32, ptr %9, align 4
  %54 = add nsw i32 %53, 1
  store i32 %54, ptr %9, align 4
  br label %40, !llvm.loop !9

55:                                               ; preds = %40
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @createStructureOne() #0 {
  %1 = alloca ptr, align 8
  %2 = call noalias ptr @malloc(i64 noundef 1500000) #4
  store ptr %2, ptr %1, align 8
  %3 = load ptr, ptr %1, align 8
  ret ptr %3
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @freeStructure(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  call void @free(ptr noundef %3) #5
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

11:                                               ; preds = %24, %4
  %12 = load i32, ptr %9, align 4
  %13 = load i32, ptr %6, align 4
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %15, label %27

15:                                               ; preds = %11
  %16 = load i32, ptr %9, align 4
  %17 = load ptr, ptr %5, align 8
  %18 = getelementptr inbounds %struct.StructureOne, ptr %17, i32 0, i32 0
  %19 = load i32, ptr %9, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [100000 x i32], ptr %18, i64 0, i64 %20
  %22 = load i32, ptr %21, align 4
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %16, i32 noundef %22)
  br label %24

24:                                               ; preds = %15
  %25 = load i32, ptr %9, align 4
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr %9, align 4
  br label %11, !llvm.loop !10

27:                                               ; preds = %11
  %28 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  store i32 0, ptr %9, align 4
  br label %29

29:                                               ; preds = %42, %27
  %30 = load i32, ptr %9, align 4
  %31 = load i32, ptr %7, align 4
  %32 = icmp slt i32 %30, %31
  br i1 %32, label %33, label %45

33:                                               ; preds = %29
  %34 = load i32, ptr %9, align 4
  %35 = load ptr, ptr %5, align 8
  %36 = getelementptr inbounds %struct.StructureOne, ptr %35, i32 0, i32 1
  %37 = load i32, ptr %9, align 4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds [200000 x i32], ptr %36, i64 0, i64 %38
  %40 = load i32, ptr %39, align 4
  %41 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %34, i32 noundef %40)
  br label %42

42:                                               ; preds = %33
  %43 = load i32, ptr %9, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, ptr %9, align 4
  br label %29, !llvm.loop !11

45:                                               ; preds = %29
  %46 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  store i32 0, ptr %9, align 4
  br label %47

47:                                               ; preds = %61, %45
  %48 = load i32, ptr %9, align 4
  %49 = load i32, ptr %8, align 4
  %50 = icmp slt i32 %48, %49
  br i1 %50, label %51, label %64

51:                                               ; preds = %47
  %52 = load i32, ptr %9, align 4
  %53 = load ptr, ptr %5, align 8
  %54 = getelementptr inbounds %struct.StructureOne, ptr %53, i32 0, i32 2
  %55 = load i32, ptr %9, align 4
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds [300000 x i8], ptr %54, i64 0, i64 %56
  %58 = load i8, ptr %57, align 1
  %59 = sext i8 %58 to i32
  %60 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %52, i32 noundef %59)
  br label %61

61:                                               ; preds = %51
  %62 = load i32, ptr %9, align 4
  %63 = add nsw i32 %62, 1
  store i32 %63, ptr %9, align 4
  br label %47, !llvm.loop !12

64:                                               ; preds = %47
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
