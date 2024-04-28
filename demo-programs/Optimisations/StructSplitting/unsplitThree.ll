; ModuleID = 'unsplitThree.c'
source_filename = "unsplitThree.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { i32, double, i32, double, i8, double, double, double, double, float, ptr }
%struct.nodeTwoOld = type { i32, double, i32, double, i8, double, double, double, double, float, ptr }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"%d\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"%f\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [8 x i8] c"%c\0A---\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 2
  store i32 9, ptr %c, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 3
  store double 2.300000e+01, ptr %d, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 4
  store i8 97, ptr %e, align 8
  %12 = load ptr, ptr %array.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %13 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom9
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 5
  store double 2.300000e+01, ptr %f, align 8
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %15 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom11
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 6
  store double 2.300000e+01, ptr %g, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwoOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwoOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx, i32 0, i32 0
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeTwoOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeTwoOld, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx4, i32 0, i32 2
  store i32 9, ptr %c, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeTwoOld, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx6, i32 0, i32 3
  store double 2.300000e+01, ptr %d, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeTwoOld, ptr %10, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx8, i32 0, i32 4
  store i8 97, ptr %e, align 8
  %12 = load ptr, ptr %array.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %13 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeTwoOld, ptr %12, i64 %idxprom9
  %f = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx10, i32 0, i32 5
  store double 2.300000e+01, ptr %f, align 8
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %15 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeTwoOld, ptr %14, i64 %idxprom11
  %g = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx12, i32 0, i32 6
  store double 2.300000e+01, ptr %g, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 40
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %5 = load double, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom4
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %8 = load i32, ptr %a, align 8
  %conv = sitofp i32 %8 to double
  %9 = call double @llvm.fmuladd.f64(double %5, double %conv, double 2.000000e+01)
  %conv6 = fptosi double %9 to i32
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %conv6, ptr %a9, align 8
  %12 = load ptr, ptr %array.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %13 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 0
  %14 = load i32, ptr %a12, align 8
  %15 = load ptr, ptr %array.addr, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %16 to i64
  %arrayidx14 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  %17 = load i32, ptr %c, align 8
  %div = sdiv i32 %14, %17
  %sub = sub nsw i32 %div, 297
  %conv15 = sitofp i32 %sub to double
  %18 = load ptr, ptr %array.addr, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %19 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %conv15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %20 = load i32, ptr %i, align 4
  %inc = add nsw i32 %20, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !7

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %21 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %21, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !8

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 30
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayOne.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %3, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %5 = load i32, ptr %a, align 8
  %6 = load ptr, ptr %arrayTwo.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeTwoOld, ptr %6, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx5, i32 0, i32 0
  %8 = load i32, ptr %a6, align 8
  %mul = mul nsw i32 %5, %8
  %9 = load ptr, ptr %arrayOne.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %10 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %mul, ptr %a9, align 8
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %12 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  %13 = load double, ptr %b, align 8
  %14 = load ptr, ptr %arrayTwo.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %15 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeTwoOld, ptr %14, i64 %idxprom12
  %b14 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx13, i32 0, i32 1
  %16 = load double, ptr %b14, align 8
  %mul15 = fmul double %13, %16
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %18 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %mul15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %19 = load i32, ptr %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %20 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %20, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !10

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %arrayOneOld = alloca ptr, align 8
  %arrayTwoOld = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 50000000, ptr %n, align 4
  %call = call i64 @malloc_usable_size(ptr noundef null) #5
  call void @free(ptr noundef null) #5
  %0 = load i32, ptr %n, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 88
  %call1 = call noalias ptr @malloc(i64 noundef %mul) #6
  store ptr %call1, ptr %arrayOneOld, align 8
  %1 = load i32, ptr %n, align 4
  %conv2 = sext i32 %1 to i64
  %mul3 = mul i64 %conv2, 88
  %call4 = call noalias ptr @malloc(i64 noundef %mul3) #6
  store ptr %call4, ptr %arrayTwoOld, align 8
  %2 = load ptr, ptr %arrayOneOld, align 8
  %3 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(ptr noundef %2, i32 noundef %3)
  %4 = load ptr, ptr %arrayTwoOld, align 8
  %5 = load i32, ptr %n, align 4
  call void @populateNodeTwoOld(ptr noundef %4, i32 noundef %5)
  %6 = load ptr, ptr %arrayOneOld, align 8
  %7 = load i32, ptr %n, align 4
  call void @multNodeOneOld(ptr noundef %6, i32 noundef %7)
  %8 = load ptr, ptr %arrayOneOld, align 8
  %9 = load ptr, ptr %arrayTwoOld, align 8
  %10 = load i32, ptr %n, align 4
  call void @multArrays(ptr noundef %8, ptr noundef %9, i32 noundef %10)
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %11 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 5
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %12 = load i32, ptr %a, align 8
  %call6 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %12)
  %13 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx7 = getelementptr inbounds %struct.nodeOneOld, ptr %13, i64 5
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx7, i32 0, i32 1
  %14 = load double, ptr %b, align 8
  %call8 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %14)
  %15 = load ptr, ptr %arrayTwoOld, align 8
  %arrayidx9 = getelementptr inbounds %struct.nodeTwoOld, ptr %15, i64 5
  %a10 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx9, i32 0, i32 0
  %16 = load i32, ptr %a10, align 8
  %call11 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %16)
  %17 = load ptr, ptr %arrayTwoOld, align 8
  %arrayidx12 = getelementptr inbounds %struct.nodeTwoOld, ptr %17, i64 5
  %b13 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx12, i32 0, i32 1
  %18 = load double, ptr %b13, align 8
  %call14 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %18)
  %19 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx15 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i64 5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx15, i32 0, i32 3
  %20 = load double, ptr %d, align 8
  %call16 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %20)
  %21 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %21, i64 5
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 4
  %22 = load i8, ptr %e, align 8
  %conv18 = sext i8 %22 to i32
  %call19 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv18)
  %23 = load ptr, ptr %arrayTwoOld, align 8
  %arrayidx20 = getelementptr inbounds %struct.nodeTwoOld, ptr %23, i64 5
  %d21 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx20, i32 0, i32 3
  %24 = load double, ptr %d21, align 8
  %call22 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %24)
  %25 = load ptr, ptr %arrayTwoOld, align 8
  %arrayidx23 = getelementptr inbounds %struct.nodeTwoOld, ptr %25, i64 5
  %e24 = getelementptr inbounds %struct.nodeTwoOld, ptr %arrayidx23, i32 0, i32 4
  %26 = load i8, ptr %e24, align 8
  %conv25 = sext i8 %26 to i32
  %call26 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv25)
  %27 = load ptr, ptr %arrayOneOld, align 8
  call void @free(ptr noundef %27) #5
  %28 = load ptr, ptr %arrayTwoOld, align 8
  call void @free(ptr noundef %28) #5
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local i64 @malloc_usable_size(ptr noundef) #2

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #2

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #3

declare dso_local i32 @printf(ptr noundef, ...) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }
attributes #6 = { nounwind allocsize(0) }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
