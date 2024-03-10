; ModuleID = 'optimised.bc'
source_filename = "unsplit.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { double, ptr, i32, i32 }
%struct.nodeOneOldCold = type { [67 x double], double, double, double, [6 x i8], i8, [4 x i8], float }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"\0Aa: ---\0A%d\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"\0Ab: ---\0A%f\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"\0Ac: ---\0A%d\00", align 1
@permitStructSplittingFlag = private constant i1 false

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc27, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end29

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %arrayOne.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 2
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %arrayOne.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 0
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %arrayOne.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 3
  store i32 9, ptr %c, align 8
  %8 = load ptr, ptr %arrayOne.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %8, i64 %idxprom5
  %cold = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 1
  %10 = call ptr @malloc(i64 584)
  store ptr %10, ptr %cold, align 8
  %11 = load ptr, ptr %cold, align 8
  %d = getelementptr inbounds %struct.nodeOneOldCold, ptr %11, i32 0, i32 4
  store i48 23, ptr %d, align 8
  %12 = load ptr, ptr %arrayOne.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %13 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom7
  %cold1 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 1
  %14 = load ptr, ptr %cold1, align 8
  %e = getelementptr inbounds %struct.nodeOneOldCold, ptr %14, i32 0, i32 5
  store i8 97, ptr %e, align 2
  store i32 0, ptr %j, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc, %for.body
  %15 = load i32, ptr %j, align 4
  %cmp10 = icmp slt i32 %15, 67
  br i1 %cmp10, label %for.body11, label %for.end

for.body11:                                       ; preds = %for.cond9
  %16 = load ptr, ptr %arrayOne.addr, align 8
  %17 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %17 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneOld, ptr %16, i64 %idxprom12
  %cold2 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 1
  %18 = load ptr, ptr %cold2, align 8
  %f = getelementptr inbounds %struct.nodeOneOldCold, ptr %18, i32 0, i32 0
  %19 = load i32, ptr %j, align 4
  %idxprom14 = sext i32 %19 to i64
  %arrayidx15 = getelementptr inbounds [67 x double], ptr %f, i64 0, i64 %idxprom14
  store double 2.300000e+01, ptr %arrayidx15, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body11
  %20 = load i32, ptr %j, align 4
  %inc = add nsw i32 %20, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond9, !llvm.loop !4

for.end:                                          ; preds = %for.cond9
  %21 = load ptr, ptr %arrayOne.addr, align 8
  %22 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %22 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %21, i64 %idxprom16
  %cold3 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  %23 = load ptr, ptr %cold3, align 8
  %g = getelementptr inbounds %struct.nodeOneOldCold, ptr %23, i32 0, i32 1
  store double 2.300000e+01, ptr %g, align 8
  %24 = load ptr, ptr %arrayTwo.addr, align 8
  %25 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %25 to i64
  %arrayidx19 = getelementptr inbounds %struct.nodeOneOld, ptr %24, i64 %idxprom18
  %a20 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 2
  store i32 1, ptr %a20, align 8
  %26 = load ptr, ptr %arrayTwo.addr, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %27 to i64
  %arrayidx22 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom21
  %b23 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 0
  store double 1.000000e+01, ptr %b23, align 8
  %28 = load ptr, ptr %arrayTwo.addr, align 8
  %29 = load i32, ptr %i, align 4
  %idxprom24 = sext i32 %29 to i64
  %arrayidx25 = getelementptr inbounds %struct.nodeOneOld, ptr %28, i64 %idxprom24
  %c26 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx25, i32 0, i32 3
  store i32 9, ptr %c26, align 8
  br label %for.inc27

for.inc27:                                        ; preds = %for.end
  %30 = load i32, ptr %i, align 4
  %inc28 = add nsw i32 %30, 1
  store i32 %inc28, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end29:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrayOne(ptr noundef %arrayOne, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
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
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %5 = load double, ptr %b, align 8
  %6 = load ptr, ptr %arrayOne.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom4
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 2
  %8 = load i32, ptr %a, align 8
  %conv = sitofp i32 %8 to double
  %9 = call double @llvm.fmuladd.f64(double %5, double %conv, double 2.000000e+01)
  %conv6 = fptosi double %9 to i32
  %10 = load ptr, ptr %arrayOne.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 2
  store i32 %conv6, ptr %a9, align 8
  %12 = load ptr, ptr %arrayOne.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %13 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 2
  %14 = load i32, ptr %a12, align 8
  %15 = load ptr, ptr %arrayOne.addr, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %16 to i64
  %arrayidx14 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 3
  %17 = load i32, ptr %c, align 8
  %div = sdiv i32 %14, %17
  %sub = sub nsw i32 %div, 297
  %conv15 = sitofp i32 %sub to double
  %18 = load ptr, ptr %arrayOne.addr, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %19 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 0
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

for.cond:                                         ; preds = %for.inc18, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end20

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
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 2
  %5 = load i32, ptr %a, align 8
  %6 = load ptr, ptr %arrayTwo.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 2
  %8 = load i32, ptr %a6, align 8
  %mul = mul nsw i32 %5, %8
  %9 = load ptr, ptr %arrayOne.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %10 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 2
  store i32 %mul, ptr %a9, align 8
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %12 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 0
  %13 = load double, ptr %b, align 8
  %14 = load ptr, ptr %arrayTwo.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %15 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom12
  %a14 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 2
  %16 = load i32, ptr %a14, align 8
  %conv = sitofp i32 %16 to double
  %add = fadd double %13, %conv
  %div = fdiv double %add, 1.100000e+00
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %18 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %idxprom15
  %b17 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx16, i32 0, i32 0
  store double %div, ptr %b17, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %19 = load i32, ptr %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc18

for.inc18:                                        ; preds = %for.end
  %20 = load i32, ptr %j, align 4
  %inc19 = add nsw i32 %20, 1
  store i32 %inc19, ptr %j, align 4
  br label %for.cond, !llvm.loop !10

for.end20:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %n = alloca i32, align 4
  %arrayOne = alloca ptr, align 8
  %arrayTwo = alloca ptr, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  %0 = load ptr, ptr %argv.addr, align 8
  %arrayidx = getelementptr inbounds ptr, ptr %0, i64 1
  %1 = load ptr, ptr %arrayidx, align 8
  %call = call i32 @atoi(ptr noundef %1) #6
  store i32 %call, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %conv = sext i32 %2 to i64
  %mul = mul i64 %conv, 600
  %3 = udiv i64 %mul, 600
  %4 = mul i64 %3, 32
  %5 = udiv i64 %4, 32
  %6 = mul i64 %5, 24
  %7 = call ptr @malloc(i64 %6)
  store ptr %7, ptr %arrayOne, align 8
  %8 = load i32, ptr %n, align 4
  %conv2 = sext i32 %8 to i64
  %mul3 = mul i64 %conv2, 600
  %9 = udiv i64 %mul3, 600
  %10 = mul i64 %9, 32
  %11 = udiv i64 %10, 32
  %12 = mul i64 %11, 24
  %13 = call ptr @malloc(i64 %12)
  store ptr %13, ptr %arrayTwo, align 8
  %14 = load ptr, ptr %arrayOne, align 8
  %15 = load ptr, ptr %arrayTwo, align 8
  %16 = load i32, ptr %n, align 4
  call void @populateNodeOne(ptr noundef %14, ptr noundef %15, i32 noundef %16)
  %call5 = call i32 @rand() #7
  %17 = load i32, ptr %n, align 4
  %rem = srem i32 %call5, %17
  store i32 %rem, ptr %i, align 4
  %call6 = call i32 @rand() #7
  %rem7 = srem i32 %call6, 67
  store i32 %rem7, ptr %j, align 4
  %18 = load ptr, ptr %arrayOne, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom = sext i32 %19 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 2
  store i32 1, ptr %a, align 8
  %20 = load ptr, ptr %arrayOne, align 8
  %21 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %21 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %20, i64 %idxprom9
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 0
  store double 1.000000e+01, ptr %b, align 8
  %22 = load ptr, ptr %arrayOne, align 8
  %23 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %23 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %22, i64 %idxprom11
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 3
  store i32 9, ptr %c, align 8
  %24 = load ptr, ptr %arrayTwo, align 8
  %25 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %25 to i64
  %arrayidx14 = getelementptr inbounds %struct.nodeOneOld, ptr %24, i64 %idxprom13
  %a15 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  store i32 1, ptr %a15, align 8
  %26 = load ptr, ptr %arrayTwo, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %27 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 0
  store double 1.000000e+01, ptr %b18, align 8
  %28 = load ptr, ptr %arrayTwo, align 8
  %29 = load i32, ptr %i, align 4
  %idxprom19 = sext i32 %29 to i64
  %arrayidx20 = getelementptr inbounds %struct.nodeOneOld, ptr %28, i64 %idxprom19
  %c21 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx20, i32 0, i32 3
  store i32 9, ptr %c21, align 8
  %30 = load ptr, ptr %arrayOne, align 8
  %31 = load i32, ptr %n, align 4
  call void @multArrayOne(ptr noundef %30, i32 noundef %31)
  %32 = load ptr, ptr %arrayOne, align 8
  %33 = load ptr, ptr %arrayTwo, align 8
  %34 = load i32, ptr %n, align 4
  call void @multArrays(ptr noundef %32, ptr noundef %33, i32 noundef %34)
  %call22 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %35 = load ptr, ptr %arrayOne, align 8
  %36 = load i32, ptr %i, align 4
  %idxprom23 = sext i32 %36 to i64
  %arrayidx24 = getelementptr inbounds %struct.nodeOneOld, ptr %35, i64 %idxprom23
  %a25 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx24, i32 0, i32 2
  %37 = load i32, ptr %a25, align 8
  %call26 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %37)
  %38 = load ptr, ptr %arrayOne, align 8
  %39 = load i32, ptr %i, align 4
  %idxprom27 = sext i32 %39 to i64
  %arrayidx28 = getelementptr inbounds %struct.nodeOneOld, ptr %38, i64 %idxprom27
  %b29 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx28, i32 0, i32 0
  %40 = load double, ptr %b29, align 8
  %call30 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %40)
  %41 = load ptr, ptr %arrayOne, align 8
  %42 = load i32, ptr %i, align 4
  %idxprom31 = sext i32 %42 to i64
  %arrayidx32 = getelementptr inbounds %struct.nodeOneOld, ptr %41, i64 %idxprom31
  %c33 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx32, i32 0, i32 3
  %43 = load i32, ptr %c33, align 8
  %call34 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %43)
  %44 = load ptr, ptr %arrayOne, align 8
  call void @freeAoSstruct.nodeOneOld(ptr %44, i64 %3)
  call void @free(ptr noundef %44) #7
  %45 = load ptr, ptr %arrayTwo, align 8
  call void @freeAoSstruct.nodeOneOld(ptr %45, i64 %9)
  call void @free(ptr noundef %45) #7
  ret i32 0
}

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef) #2

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #3

; Function Attrs: nounwind
declare dso_local i32 @rand() #4

declare dso_local i32 @printf(ptr noundef, ...) #5

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #4

define void @freeAoSstruct.nodeOneOld(ptr %0, i64 %1) {
entry:
  %aos = alloca ptr, align 8
  store ptr %0, ptr %aos, align 8
  %size = alloca i64, align 8
  store i64 %1, ptr %size, align 8
  %index = alloca i32, align 4
  store i32 0, ptr %index, align 4
  %2 = load ptr, ptr %aos, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %3 = load i32, ptr %index, align 4
  %4 = load i32, ptr %size, align 4
  %condition = icmp ne i32 %3, %4
  br i1 %condition, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %aos1 = load ptr, ptr %aos, align 8
  %5 = load i32, ptr %index, align 4
  %6 = sext i32 %5 to i64
  %elemAccess = getelementptr inbounds %struct.nodeOneOld, ptr %aos1, i64 %6
  %coldStructPtr = getelementptr inbounds %struct.nodeOneOld, ptr %elemAccess, i32 0, i32 1
  %coldStruct = load ptr, ptr %coldStructPtr, align 8
  call void @free(ptr %coldStruct)
  %7 = load i32, ptr %index, align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, ptr %index, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind willreturn memory(read) }
attributes #7 = { nounwind }

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
