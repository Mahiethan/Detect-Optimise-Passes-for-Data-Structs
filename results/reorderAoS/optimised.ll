; ModuleID = 'optimised.bc'
source_filename = "unordered.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOne = type { double, i32, i8, i8 }
%struct.nodeTwo = type { double, [6 x i8], i8, i32, i32 }
%struct.nodeThree = type { [96 x float], double, i32, i32 }
%struct.nodeFour = type { [11 x i32], [111 x float], [4 x i8], [1 x i32] }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"a: %d\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"b: %f\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"c: %c\0A---\0A\00", align 1
@.str.4 = private unnamed_addr constant [11 x i8] c"d: %c\0A---\0A\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"a: %f\0A---\0A\00", align 1
@.str.6 = private unnamed_addr constant [13 x i8] c"b: %lld\0A---\0A\00", align 1
@.str.7 = private unnamed_addr constant [11 x i8] c"c: %d\0A---\0A\00", align 1
@.str.8 = private unnamed_addr constant [11 x i8] c"d: %d\0A---\0A\00", align 1
@.str.9 = private unnamed_addr constant [11 x i8] c"e: %c\0A---\0A\00", align 1
@.str.10 = private unnamed_addr constant [11 x i8] c"a: %c\0A---\0A\00", align 1
@.str.11 = private unnamed_addr constant [11 x i8] c"d: %f\0A---\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %array, i32 noundef %size) #0 {
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
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 1
  store i32 17, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOne, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOne, ptr %arrayidx2, i32 0, i32 0
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOne, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOne, ptr %arrayidx4, i32 0, i32 2
  store i8 99, ptr %c, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOne, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOne, ptr %arrayidx6, i32 0, i32 3
  store i8 100, ptr %d, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32, ptr %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOne(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %random = alloca float, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  %call = call i32 @rand() #7
  %conv = sitofp i32 %call to float
  %div = fdiv float %conv, 0x41B99999A0000000
  store float %div, ptr %random, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  %5 = load double, ptr %b, align 8
  %6 = load float, ptr %random, align 4
  %conv5 = fpext float %6 to double
  %div6 = fdiv double 2.000000e-01, %conv5
  %add = fadd double %5, %div6
  %conv7 = fptosi double %add to i32
  %7 = load ptr, ptr %array.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %8 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOne, ptr %7, i64 %idxprom8
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx9, i32 0, i32 1
  store i32 %conv7, ptr %a, align 8
  %9 = load ptr, ptr %array.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %10 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOne, ptr %9, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx11, i32 0, i32 1
  %11 = load i32, ptr %a12, align 8
  %conv13 = sitofp i32 %11 to double
  %12 = load float, ptr %random, align 4
  %conv14 = fpext float %12 to double
  %13 = call double @llvm.fmuladd.f64(double %conv14, double 2.000000e-01, double %conv13)
  %div15 = fdiv double %13, 2.000000e-01
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %15 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOne, ptr %14, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx17, i32 0, i32 0
  store double %div15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !6

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %17 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %17, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !7

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @rand() #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
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

for.cond:                                         ; preds = %for.inc22, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end24

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
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %3, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 1
  %5 = load i32, ptr %a, align 8
  %conv = sitofp i32 %5 to double
  %6 = load ptr, ptr %arrayTwo.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOne, ptr %6, i64 %idxprom4
  %b = getelementptr inbounds %struct.nodeOne, ptr %arrayidx5, i32 0, i32 0
  %8 = load double, ptr %b, align 8
  %mul = fmul double %8, 1.000000e-01
  %mul6 = fmul double %conv, %mul
  %conv7 = fptosi double %mul6 to i32
  %9 = load ptr, ptr %arrayOne.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %10 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOne, ptr %9, i64 %idxprom8
  %a10 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx9, i32 0, i32 1
  store i32 %conv7, ptr %a10, align 8
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %12 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOne, ptr %11, i64 %idxprom11
  %b13 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx12, i32 0, i32 0
  %13 = load double, ptr %b13, align 8
  %14 = load ptr, ptr %arrayTwo.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom14 = sext i32 %15 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeOne, ptr %14, i64 %idxprom14
  %b16 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx15, i32 0, i32 0
  %16 = load double, ptr %b16, align 8
  %mul17 = fmul double %16, 1.000000e-01
  %mul18 = fmul double %13, %mul17
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom19 = sext i32 %18 to i64
  %arrayidx20 = getelementptr inbounds %struct.nodeOne, ptr %17, i64 %idxprom19
  %b21 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx20, i32 0, i32 0
  store double %mul18, ptr %b21, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %19 = load i32, ptr %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !8

for.end:                                          ; preds = %for.cond1
  br label %for.inc22

for.inc22:                                        ; preds = %for.end
  %20 = load i32, ptr %j, align 4
  %inc23 = add nsw i32 %20, 1
  store i32 %inc23, ptr %j, align 4
  br label %for.cond, !llvm.loop !9

for.end24:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwo(ptr noundef %array, i32 noundef %size) #0 {
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
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store double 3.200000e+00, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeTwo, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx2, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.clear = and i64 %bf.load, -281474976710656
  %bf.set = or i64 %bf.clear, 1000
  %6 = trunc i64 %bf.set to i48
  store i48 %6, ptr %b, align 8
  %7 = load ptr, ptr %array.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %8 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeTwo, ptr %7, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx4, i32 0, i32 3
  store i32 77, ptr %c, align 8
  %9 = load ptr, ptr %array.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %10 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeTwo, ptr %9, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx6, i32 0, i32 4
  store i32 10, ptr %d, align 4
  %11 = load ptr, ptr %array.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %12 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeTwo, ptr %11, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx8, i32 0, i32 2
  store i8 100, ptr %e, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %13 = load i32, ptr %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !10

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeTwo(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %random = alloca float, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc20, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  %call = call i32 @rand() #7
  %conv = sitofp i32 %call to float
  %div = fdiv float %conv, 0x41B99999A0000000
  store float %div, ptr %random, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %conv5 = sitofp i64 %bf.ashr to double
  %5 = load float, ptr %random, align 4
  %conv6 = fpext float %5 to double
  %div7 = fdiv double 2.000000e-01, %conv6
  %add = fadd double %conv5, %div7
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %7 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeTwo, ptr %6, i64 %idxprom8
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx9, i32 0, i32 0
  store double %add, ptr %a, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeTwo, ptr %8, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx11, i32 0, i32 0
  %10 = load double, ptr %a12, align 8
  %11 = load float, ptr %random, align 4
  %conv13 = fpext float %11 to double
  %12 = call double @llvm.fmuladd.f64(double %conv13, double 2.000000e-01, double %10)
  %div14 = fdiv double %12, 2.000000e-01
  %conv15 = fptosi double %div14 to i64
  %13 = load ptr, ptr %array.addr, align 8
  %14 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %14 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeTwo, ptr %13, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx17, i32 0, i32 1
  %bf.load19 = load i64, ptr %b18, align 8
  %bf.value = and i64 %conv15, 281474976710655
  %bf.clear = and i64 %bf.load19, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %15 = trunc i64 %bf.set to i48
  store i48 %15, ptr %b18, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !11

for.end:                                          ; preds = %for.cond1
  br label %for.inc20

for.inc20:                                        ; preds = %for.end
  %17 = load i32, ptr %j, align 4
  %inc21 = add nsw i32 %17, 1
  store i32 %inc21, ptr %j, align 4
  br label %for.cond, !llvm.loop !12

for.end22:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeTwoArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %randomletter = alloca i8, align 1
  %i = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc17, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end19

for.body:                                         ; preds = %for.cond
  %call = call i64 @random() #7
  %rem = srem i64 %call, 26
  %add = add nsw i64 65, %rem
  %conv = trunc i64 %add to i8
  store i8 %conv, ptr %randomletter, align 1
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayOne.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %5 = load ptr, ptr %arrayTwo.addr, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %6 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeTwo, ptr %5, i64 %idxprom5
  %b7 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx6, i32 0, i32 1
  %bf.load8 = load i64, ptr %b7, align 8
  %bf.shl9 = shl i64 %bf.load8, 16
  %bf.ashr10 = ashr i64 %bf.shl9, 16
  %mul = mul nsw i64 %bf.ashr, %bf.ashr10
  %7 = load ptr, ptr %arrayOne.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %8 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeTwo, ptr %7, i64 %idxprom11
  %b13 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx12, i32 0, i32 1
  %bf.load14 = load i64, ptr %b13, align 8
  %bf.value = and i64 %mul, 281474976710655
  %bf.clear = and i64 %bf.load14, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %9 = trunc i64 %bf.set to i48
  store i48 %9, ptr %b13, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  %10 = load i8, ptr %randomletter, align 1
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %12 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeTwo, ptr %11, i64 %idxprom15
  %e = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx16, i32 0, i32 2
  store i8 %10, ptr %e, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %13 = load i32, ptr %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !13

for.end:                                          ; preds = %for.cond1
  br label %for.inc17

for.inc17:                                        ; preds = %for.end
  %14 = load i32, ptr %j, align 4
  %inc18 = add nsw i32 %14, 1
  store i32 %inc18, ptr %j, align 4
  br label %for.cond, !llvm.loop !14

for.end19:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind
declare dso_local i64 @random() #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeThree(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %random = alloca float, align 4
  %j = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc13, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end15

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeThree, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeThree, ptr %arrayidx, i32 0, i32 2
  store i32 54, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeThree, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeThree, ptr %arrayidx2, i32 0, i32 1
  store double 1.001000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeThree, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeThree, ptr %arrayidx4, i32 0, i32 3
  store i32 77, ptr %c, align 8
  %call = call i32 @rand() #7
  %conv = sitofp i32 %call to float
  %div = fdiv float %conv, 0x41B99999A0000000
  store float %div, ptr %random, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %for.body
  %8 = load i32, ptr %j, align 4
  %cmp6 = icmp slt i32 %8, 96
  br i1 %cmp6, label %for.body8, label %for.end

for.body8:                                        ; preds = %for.cond5
  %9 = load float, ptr %random, align 4
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %11 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeThree, ptr %10, i64 %idxprom9
  %d = getelementptr inbounds %struct.nodeThree, ptr %arrayidx10, i32 0, i32 0
  %12 = load i32, ptr %j, align 4
  %idxprom11 = sext i32 %12 to i64
  %arrayidx12 = getelementptr inbounds [96 x float], ptr %d, i64 0, i64 %idxprom11
  store float %9, ptr %arrayidx12, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body8
  %13 = load i32, ptr %j, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond5, !llvm.loop !15

for.end:                                          ; preds = %for.cond5
  br label %for.inc13

for.inc13:                                        ; preds = %for.end
  %14 = load i32, ptr %i, align 4
  %inc14 = add nsw i32 %14, 1
  store i32 %inc14, ptr %i, align 4
  br label %for.cond, !llvm.loop !16

for.end15:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeThree(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %random = alloca float, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc18, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end20

for.body:                                         ; preds = %for.cond
  %call = call i32 @rand() #7
  %conv = sitofp i32 %call to float
  %div = fdiv float %conv, 0x41B99999A0000000
  store float %div, ptr %random, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeThree, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeThree, ptr %arrayidx, i32 0, i32 1
  %5 = load double, ptr %b, align 8
  %6 = load float, ptr %random, align 4
  %conv5 = fpext float %6 to double
  %div6 = fdiv double 2.000000e-01, %conv5
  %add = fadd double %5, %div6
  %7 = load ptr, ptr %array.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %8 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeThree, ptr %7, i64 %idxprom7
  %b9 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx8, i32 0, i32 1
  store double %add, ptr %b9, align 8
  %9 = load ptr, ptr %array.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %10 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeThree, ptr %9, i64 %idxprom10
  %b12 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx11, i32 0, i32 1
  %11 = load double, ptr %b12, align 8
  %12 = load float, ptr %random, align 4
  %conv13 = fpext float %12 to double
  %13 = call double @llvm.fmuladd.f64(double %conv13, double 2.000000e-01, double %11)
  %div14 = fdiv double %13, 2.000000e-01
  %conv15 = fptosi double %div14 to i32
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %15 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeThree, ptr %14, i64 %idxprom16
  %c = getelementptr inbounds %struct.nodeThree, ptr %arrayidx17, i32 0, i32 3
  store i32 %conv15, ptr %c, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %16 = load i32, ptr %i, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !17

for.end:                                          ; preds = %for.cond1
  br label %for.inc18

for.inc18:                                        ; preds = %for.end
  %17 = load i32, ptr %j, align 4
  %inc19 = add nsw i32 %17, 1
  store i32 %inc19, ptr %j, align 4
  br label %for.cond, !llvm.loop !18

for.end20:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeThreeArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %k = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc11, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end13

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %k, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %k, align 4
  %cmp2 = icmp slt i32 %2, 96
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayTwo.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeThree, ptr %3, i64 %idxprom
  %d = getelementptr inbounds %struct.nodeThree, ptr %arrayidx, i32 0, i32 0
  %5 = load i32, ptr %k, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [96 x float], ptr %d, i64 0, i64 %idxprom4
  %6 = load float, ptr %arrayidx5, align 4
  %7 = load ptr, ptr %arrayOne.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom6 = sext i32 %8 to i64
  %arrayidx7 = getelementptr inbounds %struct.nodeThree, ptr %7, i64 %idxprom6
  %d8 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx7, i32 0, i32 0
  %9 = load i32, ptr %k, align 4
  %idxprom9 = sext i32 %9 to i64
  %arrayidx10 = getelementptr inbounds [96 x float], ptr %d8, i64 0, i64 %idxprom9
  %10 = load float, ptr %arrayidx10, align 4
  %add = fadd float %10, %6
  store float %add, ptr %arrayidx10, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %11 = load i32, ptr %k, align 4
  %inc = add nsw i32 %11, 1
  store i32 %inc, ptr %k, align 4
  br label %for.cond1, !llvm.loop !19

for.end:                                          ; preds = %for.cond1
  br label %for.inc11

for.inc11:                                        ; preds = %for.end
  %12 = load i32, ptr %i, align 4
  %inc12 = add nsw i32 %12, 1
  store i32 %inc12, ptr %i, align 4
  br label %for.cond, !llvm.loop !20

for.end13:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeFour(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc30, %entry
  %0 = load i32, ptr %j, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end32

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %i, align 4
  %cmp2 = icmp slt i32 %2, 11
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %j, align 4
  %idxprom = sext i32 %5 to i64
  %arrayidx = getelementptr inbounds %struct.nodeFour, ptr %4, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeFour, ptr %arrayidx, i32 0, i32 0
  %6 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %6 to i64
  %arrayidx5 = getelementptr inbounds [11 x i32], ptr %a, i64 0, i64 %idxprom4
  store i32 %3, ptr %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %7 = load i32, ptr %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !21

for.end:                                          ; preds = %for.cond1
  store i32 0, ptr %i, align 4
  br label %for.cond6

for.cond6:                                        ; preds = %for.inc13, %for.end
  %8 = load i32, ptr %i, align 4
  %cmp7 = icmp slt i32 %8, 111
  br i1 %cmp7, label %for.body8, label %for.end15

for.body8:                                        ; preds = %for.cond6
  %9 = load i32, ptr %i, align 4
  %conv = sitofp i32 %9 to float
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %j, align 4
  %idxprom9 = sext i32 %11 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeFour, ptr %10, i64 %idxprom9
  %b = getelementptr inbounds %struct.nodeFour, ptr %arrayidx10, i32 0, i32 1
  %12 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %12 to i64
  %arrayidx12 = getelementptr inbounds [111 x float], ptr %b, i64 0, i64 %idxprom11
  store float %conv, ptr %arrayidx12, align 4
  br label %for.inc13

for.inc13:                                        ; preds = %for.body8
  %13 = load i32, ptr %i, align 4
  %inc14 = add nsw i32 %13, 1
  store i32 %inc14, ptr %i, align 4
  br label %for.cond6, !llvm.loop !22

for.end15:                                        ; preds = %for.cond6
  store i32 0, ptr %i, align 4
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc24, %for.end15
  %14 = load i32, ptr %i, align 4
  %cmp17 = icmp slt i32 %14, 4
  br i1 %cmp17, label %for.body19, label %for.end26

for.body19:                                       ; preds = %for.cond16
  %15 = load ptr, ptr %array.addr, align 8
  %16 = load i32, ptr %j, align 4
  %idxprom20 = sext i32 %16 to i64
  %arrayidx21 = getelementptr inbounds %struct.nodeFour, ptr %15, i64 %idxprom20
  %c = getelementptr inbounds %struct.nodeFour, ptr %arrayidx21, i32 0, i32 2
  %17 = load i32, ptr %i, align 4
  %idxprom22 = sext i32 %17 to i64
  %arrayidx23 = getelementptr inbounds [4 x i8], ptr %c, i64 0, i64 %idxprom22
  store i8 103, ptr %arrayidx23, align 1
  br label %for.inc24

for.inc24:                                        ; preds = %for.body19
  %18 = load i32, ptr %i, align 4
  %inc25 = add nsw i32 %18, 1
  store i32 %inc25, ptr %i, align 4
  br label %for.cond16, !llvm.loop !23

for.end26:                                        ; preds = %for.cond16
  %19 = load i32, ptr %i, align 4
  %20 = load ptr, ptr %array.addr, align 8
  %21 = load i32, ptr %j, align 4
  %idxprom27 = sext i32 %21 to i64
  %arrayidx28 = getelementptr inbounds %struct.nodeFour, ptr %20, i64 %idxprom27
  %d = getelementptr inbounds %struct.nodeFour, ptr %arrayidx28, i32 0, i32 3
  %arrayidx29 = getelementptr inbounds [1 x i32], ptr %d, i64 0, i64 0
  store i32 %19, ptr %arrayidx29, align 4
  br label %for.inc30

for.inc30:                                        ; preds = %for.end26
  %22 = load i32, ptr %j, align 4
  %inc31 = add nsw i32 %22, 1
  store i32 %inc31, ptr %j, align 4
  br label %for.cond, !llvm.loop !24

for.end32:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeFourArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
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

for.cond:                                         ; preds = %for.inc36, %entry
  %0 = load i32, ptr %j, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end38

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %i, align 4
  %cmp2 = icmp slt i32 %2, 11
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayTwo.addr, align 8
  %4 = load i32, ptr %j, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeFour, ptr %3, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeFour, ptr %arrayidx, i32 0, i32 0
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [11 x i32], ptr %a, i64 0, i64 %idxprom4
  %6 = load i32, ptr %arrayidx5, align 4
  %7 = load ptr, ptr %arrayOne.addr, align 8
  %8 = load i32, ptr %j, align 4
  %idxprom6 = sext i32 %8 to i64
  %arrayidx7 = getelementptr inbounds %struct.nodeFour, ptr %7, i64 %idxprom6
  %a8 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx7, i32 0, i32 0
  %9 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %9 to i64
  %arrayidx10 = getelementptr inbounds [11 x i32], ptr %a8, i64 0, i64 %idxprom9
  %10 = load i32, ptr %arrayidx10, align 4
  %mul = mul nsw i32 %10, %6
  store i32 %mul, ptr %arrayidx10, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %11 = load i32, ptr %i, align 4
  %inc = add nsw i32 %11, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !25

for.end:                                          ; preds = %for.cond1
  store i32 0, ptr %i, align 4
  br label %for.cond11

for.cond11:                                       ; preds = %for.inc24, %for.end
  %12 = load i32, ptr %i, align 4
  %cmp12 = icmp slt i32 %12, 111
  br i1 %cmp12, label %for.body13, label %for.end26

for.body13:                                       ; preds = %for.cond11
  %13 = load ptr, ptr %arrayTwo.addr, align 8
  %14 = load i32, ptr %j, align 4
  %idxprom14 = sext i32 %14 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeFour, ptr %13, i64 %idxprom14
  %a16 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx15, i32 0, i32 0
  %15 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %15 to i64
  %arrayidx18 = getelementptr inbounds [11 x i32], ptr %a16, i64 0, i64 %idxprom17
  %16 = load i32, ptr %arrayidx18, align 4
  %conv = sitofp i32 %16 to float
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %j, align 4
  %idxprom19 = sext i32 %18 to i64
  %arrayidx20 = getelementptr inbounds %struct.nodeFour, ptr %17, i64 %idxprom19
  %b = getelementptr inbounds %struct.nodeFour, ptr %arrayidx20, i32 0, i32 1
  %19 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %19 to i64
  %arrayidx22 = getelementptr inbounds [111 x float], ptr %b, i64 0, i64 %idxprom21
  %20 = load float, ptr %arrayidx22, align 4
  %mul23 = fmul float %20, %conv
  store float %mul23, ptr %arrayidx22, align 4
  br label %for.inc24

for.inc24:                                        ; preds = %for.body13
  %21 = load i32, ptr %i, align 4
  %inc25 = add nsw i32 %21, 1
  store i32 %inc25, ptr %i, align 4
  br label %for.cond11, !llvm.loop !26

for.end26:                                        ; preds = %for.cond11
  %22 = load ptr, ptr %arrayTwo.addr, align 8
  %23 = load i32, ptr %j, align 4
  %idxprom27 = sext i32 %23 to i64
  %arrayidx28 = getelementptr inbounds %struct.nodeFour, ptr %22, i64 %idxprom27
  %a29 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx28, i32 0, i32 0
  %24 = load i32, ptr %i, align 4
  %idxprom30 = sext i32 %24 to i64
  %arrayidx31 = getelementptr inbounds [11 x i32], ptr %a29, i64 0, i64 %idxprom30
  %25 = load i32, ptr %arrayidx31, align 4
  %26 = load ptr, ptr %arrayOne.addr, align 8
  %27 = load i32, ptr %j, align 4
  %idxprom32 = sext i32 %27 to i64
  %arrayidx33 = getelementptr inbounds %struct.nodeFour, ptr %26, i64 %idxprom32
  %d = getelementptr inbounds %struct.nodeFour, ptr %arrayidx33, i32 0, i32 3
  %arrayidx34 = getelementptr inbounds [1 x i32], ptr %d, i64 0, i64 0
  %28 = load i32, ptr %arrayidx34, align 4
  %mul35 = mul nsw i32 %28, %25
  store i32 %mul35, ptr %arrayidx34, align 4
  br label %for.inc36

for.inc36:                                        ; preds = %for.end26
  %29 = load i32, ptr %j, align 4
  %inc37 = add nsw i32 %29, 1
  store i32 %inc37, ptr %j, align 4
  br label %for.cond, !llvm.loop !27

for.end38:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %n = alloca i32, align 4
  %randomIndex = alloca i32, align 4
  %arrayOne = alloca ptr, align 8
  %arrayTwo = alloca ptr, align 8
  %arrayThree = alloca ptr, align 8
  %arrayFour = alloca ptr, align 8
  %arrayFive = alloca ptr, align 8
  %arraySix = alloca ptr, align 8
  %randomDIndex = alloca i32, align 4
  %arraySeven = alloca ptr, align 8
  %arrayEight = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  %0 = load ptr, ptr %argv.addr, align 8
  %arrayidx = getelementptr inbounds ptr, ptr %0, i64 1
  %1 = load ptr, ptr %arrayidx, align 8
  %call = call i32 @atoi(ptr noundef %1) #8
  store i32 %call, ptr %n, align 4
  %call1 = call i32 @rand() #7
  %2 = load i32, ptr %n, align 4
  %rem = srem i32 %call1, %2
  store i32 %rem, ptr %randomIndex, align 4
  %3 = load i32, ptr %n, align 4
  %conv = sext i32 %3 to i64
  %call2 = call noalias ptr @calloc(i64 noundef %conv, i64 noundef 16) #9
  store ptr %call2, ptr %arrayOne, align 8
  %4 = load i32, ptr %n, align 4
  %conv3 = sext i32 %4 to i64
  %mul = mul i64 %conv3, 24
  %5 = udiv i64 %mul, 24
  %6 = mul i64 %5, 16
  %7 = call ptr @malloc(i64 %6)
  store ptr %7, ptr %arrayTwo, align 8
  %8 = load ptr, ptr %arrayOne, align 8
  %9 = load i32, ptr %n, align 4
  call void @populateNodeOne(ptr noundef %8, i32 noundef %9)
  %10 = load ptr, ptr %arrayTwo, align 8
  %11 = load i32, ptr %n, align 4
  call void @populateNodeOne(ptr noundef %10, i32 noundef %11)
  %12 = load ptr, ptr %arrayOne, align 8
  %13 = load i32, ptr %n, align 4
  call void @multNodeOne(ptr noundef %12, i32 noundef %13)
  %14 = load ptr, ptr %arrayOne, align 8
  %15 = load ptr, ptr %arrayTwo, align 8
  %16 = load i32, ptr %n, align 4
  call void @multNodeOneArrays(ptr noundef %14, ptr noundef %15, i32 noundef %16)
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %17 = load ptr, ptr %arrayOne, align 8
  %18 = load i32, ptr %randomIndex, align 4
  %idxprom = sext i32 %18 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOne, ptr %17, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx6, i32 0, i32 1
  %19 = load i32, ptr %a, align 8
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %19)
  %20 = load ptr, ptr %arrayOne, align 8
  %21 = load i32, ptr %randomIndex, align 4
  %idxprom8 = sext i32 %21 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOne, ptr %20, i64 %idxprom8
  %b = getelementptr inbounds %struct.nodeOne, ptr %arrayidx9, i32 0, i32 0
  %22 = load double, ptr %b, align 8
  %call10 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %22)
  %23 = load ptr, ptr %arrayOne, align 8
  %24 = load i32, ptr %randomIndex, align 4
  %idxprom11 = sext i32 %24 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOne, ptr %23, i64 %idxprom11
  %c = getelementptr inbounds %struct.nodeOne, ptr %arrayidx12, i32 0, i32 2
  %25 = load i8, ptr %c, align 8
  %conv13 = sext i8 %25 to i32
  %call14 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv13)
  %26 = load ptr, ptr %arrayOne, align 8
  %27 = load i32, ptr %randomIndex, align 4
  %idxprom15 = sext i32 %27 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeOne, ptr %26, i64 %idxprom15
  %d = getelementptr inbounds %struct.nodeOne, ptr %arrayidx16, i32 0, i32 3
  %28 = load i8, ptr %d, align 1
  %conv17 = sext i8 %28 to i32
  %call18 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %conv17)
  %29 = load ptr, ptr %arrayOne, align 8
  call void @free(ptr noundef %29) #7
  %30 = load ptr, ptr %arrayTwo, align 8
  call void @free(ptr noundef %30) #7
  %31 = load i32, ptr %n, align 4
  %conv19 = sext i32 %31 to i64
  %mul20 = mul i64 %conv19, 32
  %32 = udiv i64 %mul20, 32
  %33 = mul i64 %32, 24
  %34 = call ptr @malloc(i64 %33)
  store ptr %34, ptr %arrayThree, align 8
  %35 = load i32, ptr %n, align 4
  %conv22 = sext i32 %35 to i64
  %mul23 = mul i64 %conv22, 32
  %36 = udiv i64 %mul23, 32
  %37 = mul i64 %36, 24
  %38 = call ptr @malloc(i64 %37)
  store ptr %38, ptr %arrayFour, align 8
  %39 = load ptr, ptr %arrayThree, align 8
  %40 = load i32, ptr %n, align 4
  call void @populateNodeTwo(ptr noundef %39, i32 noundef %40)
  %41 = load ptr, ptr %arrayFour, align 8
  %42 = load i32, ptr %n, align 4
  call void @populateNodeTwo(ptr noundef %41, i32 noundef %42)
  %43 = load ptr, ptr %arrayThree, align 8
  %44 = load i32, ptr %n, align 4
  call void @multNodeTwo(ptr noundef %43, i32 noundef %44)
  %45 = load ptr, ptr %arrayThree, align 8
  %46 = load ptr, ptr %arrayFour, align 8
  %47 = load i32, ptr %n, align 4
  call void @multNodeTwoArrays(ptr noundef %45, ptr noundef %46, i32 noundef %47)
  %call25 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %48 = load ptr, ptr %arrayThree, align 8
  %49 = load i32, ptr %randomIndex, align 4
  %idxprom26 = sext i32 %49 to i64
  %arrayidx27 = getelementptr inbounds %struct.nodeTwo, ptr %48, i64 %idxprom26
  %a28 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx27, i32 0, i32 0
  %50 = load double, ptr %a28, align 8
  %call29 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, double noundef %50)
  %51 = load ptr, ptr %arrayThree, align 8
  %52 = load i32, ptr %randomIndex, align 4
  %idxprom30 = sext i32 %52 to i64
  %arrayidx31 = getelementptr inbounds %struct.nodeTwo, ptr %51, i64 %idxprom30
  %b32 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx31, i32 0, i32 1
  %bf.load = load i64, ptr %b32, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %call33 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, i64 noundef %bf.ashr)
  %53 = load ptr, ptr %arrayThree, align 8
  %54 = load i32, ptr %randomIndex, align 4
  %idxprom34 = sext i32 %54 to i64
  %arrayidx35 = getelementptr inbounds %struct.nodeTwo, ptr %53, i64 %idxprom34
  %c36 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx35, i32 0, i32 3
  %55 = load i32, ptr %c36, align 8
  %call37 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %55)
  %56 = load ptr, ptr %arrayThree, align 8
  %57 = load i32, ptr %randomIndex, align 4
  %idxprom38 = sext i32 %57 to i64
  %arrayidx39 = getelementptr inbounds %struct.nodeTwo, ptr %56, i64 %idxprom38
  %d40 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx39, i32 0, i32 4
  %58 = load i32, ptr %d40, align 4
  %call41 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, i32 noundef %58)
  %59 = load ptr, ptr %arrayThree, align 8
  %60 = load i32, ptr %randomIndex, align 4
  %idxprom42 = sext i32 %60 to i64
  %arrayidx43 = getelementptr inbounds %struct.nodeTwo, ptr %59, i64 %idxprom42
  %e = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx43, i32 0, i32 2
  %61 = load i8, ptr %e, align 8
  %conv44 = sext i8 %61 to i32
  %call45 = call i32 (ptr, ...) @printf(ptr noundef @.str.9, i32 noundef %conv44)
  %62 = load ptr, ptr %arrayThree, align 8
  call void @free(ptr noundef %62) #7
  %63 = load ptr, ptr %arrayFour, align 8
  call void @free(ptr noundef %63) #7
  %64 = load i32, ptr %n, align 4
  %conv46 = sext i32 %64 to i64
  %call47 = call noalias ptr @calloc(i64 noundef %conv46, i64 noundef 400) #9
  store ptr %call47, ptr %arrayFive, align 8
  %65 = load i32, ptr %n, align 4
  %conv48 = sext i32 %65 to i64
  %mul49 = mul i64 %conv48, 408
  %66 = udiv i64 %mul49, 408
  %67 = mul i64 %66, 400
  %68 = call ptr @malloc(i64 %67)
  store ptr %68, ptr %arraySix, align 8
  %69 = load ptr, ptr %arrayFive, align 8
  %70 = load i32, ptr %n, align 4
  call void @populateNodeThree(ptr noundef %69, i32 noundef %70)
  %71 = load ptr, ptr %arraySix, align 8
  %72 = load i32, ptr %n, align 4
  call void @populateNodeThree(ptr noundef %71, i32 noundef %72)
  %73 = load ptr, ptr %arrayFive, align 8
  %74 = load i32, ptr %n, align 4
  call void @multNodeThree(ptr noundef %73, i32 noundef %74)
  %75 = load ptr, ptr %arrayFive, align 8
  %76 = load ptr, ptr %arraySix, align 8
  %77 = load i32, ptr %n, align 4
  call void @multNodeThreeArrays(ptr noundef %75, ptr noundef %76, i32 noundef %77)
  %call51 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %call52 = call i32 @rand() #7
  %rem53 = srem i32 %call52, 97
  store i32 %rem53, ptr %randomDIndex, align 4
  %78 = load ptr, ptr %arrayFive, align 8
  %79 = load i32, ptr %randomIndex, align 4
  %idxprom54 = sext i32 %79 to i64
  %arrayidx55 = getelementptr inbounds %struct.nodeThree, ptr %78, i64 %idxprom54
  %a56 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx55, i32 0, i32 2
  %80 = load i32, ptr %a56, align 8
  %call57 = call i32 (ptr, ...) @printf(ptr noundef @.str.10, i32 noundef %80)
  %81 = load ptr, ptr %arrayFive, align 8
  %82 = load i32, ptr %randomIndex, align 4
  %idxprom58 = sext i32 %82 to i64
  %arrayidx59 = getelementptr inbounds %struct.nodeThree, ptr %81, i64 %idxprom58
  %b60 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx59, i32 0, i32 1
  %83 = load double, ptr %b60, align 8
  %call61 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %83)
  %84 = load ptr, ptr %arrayFive, align 8
  %85 = load i32, ptr %randomIndex, align 4
  %idxprom62 = sext i32 %85 to i64
  %arrayidx63 = getelementptr inbounds %struct.nodeThree, ptr %84, i64 %idxprom62
  %c64 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx63, i32 0, i32 3
  %86 = load i32, ptr %c64, align 8
  %call65 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %86)
  %87 = load ptr, ptr %arrayFive, align 8
  %88 = load i32, ptr %randomIndex, align 4
  %idxprom66 = sext i32 %88 to i64
  %arrayidx67 = getelementptr inbounds %struct.nodeThree, ptr %87, i64 %idxprom66
  %d68 = getelementptr inbounds %struct.nodeThree, ptr %arrayidx67, i32 0, i32 0
  %89 = load i32, ptr %randomDIndex, align 4
  %idxprom69 = sext i32 %89 to i64
  %arrayidx70 = getelementptr inbounds [96 x float], ptr %d68, i64 0, i64 %idxprom69
  %90 = load float, ptr %arrayidx70, align 4
  %conv71 = fpext float %90 to double
  %call72 = call i32 (ptr, ...) @printf(ptr noundef @.str.11, double noundef %conv71)
  %91 = load ptr, ptr %arrayFive, align 8
  call void @free(ptr noundef %91) #7
  %92 = load ptr, ptr %arraySix, align 8
  call void @free(ptr noundef %92) #7
  %93 = load i32, ptr %n, align 4
  %conv73 = sext i32 %93 to i64
  %call74 = call noalias ptr @calloc(i64 noundef %conv73, i64 noundef 496) #9
  store ptr %call74, ptr %arraySeven, align 8
  %94 = load i32, ptr %n, align 4
  %conv75 = sext i32 %94 to i64
  %mul76 = mul i64 %conv75, 496
  %call77 = call noalias ptr @malloc(i64 noundef %mul76) #10
  store ptr %call77, ptr %arrayEight, align 8
  %95 = load ptr, ptr %arraySeven, align 8
  %96 = load i32, ptr %n, align 4
  call void @populateNodeFour(ptr noundef %95, i32 noundef %96)
  %97 = load ptr, ptr %arrayEight, align 8
  %98 = load i32, ptr %n, align 4
  call void @populateNodeFour(ptr noundef %97, i32 noundef %98)
  %99 = load ptr, ptr %arraySeven, align 8
  %100 = load ptr, ptr %arrayEight, align 8
  %101 = load i32, ptr %n, align 4
  call void @multNodeFourArrays(ptr noundef %99, ptr noundef %100, i32 noundef %101)
  %call78 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %102 = load ptr, ptr %arraySeven, align 8
  %103 = load i32, ptr %randomIndex, align 4
  %idxprom79 = sext i32 %103 to i64
  %arrayidx80 = getelementptr inbounds %struct.nodeFour, ptr %102, i64 %idxprom79
  %a81 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx80, i32 0, i32 0
  %arrayidx82 = getelementptr inbounds [11 x i32], ptr %a81, i64 0, i64 3
  %104 = load i32, ptr %arrayidx82, align 4
  %call83 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %104)
  %105 = load ptr, ptr %arraySeven, align 8
  %106 = load i32, ptr %randomIndex, align 4
  %idxprom84 = sext i32 %106 to i64
  %arrayidx85 = getelementptr inbounds %struct.nodeFour, ptr %105, i64 %idxprom84
  %b86 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx85, i32 0, i32 1
  %arrayidx87 = getelementptr inbounds [111 x float], ptr %b86, i64 0, i64 57
  %107 = load float, ptr %arrayidx87, align 4
  %conv88 = fpext float %107 to double
  %call89 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %conv88)
  %108 = load ptr, ptr %arraySeven, align 8
  %109 = load i32, ptr %randomIndex, align 4
  %idxprom90 = sext i32 %109 to i64
  %arrayidx91 = getelementptr inbounds %struct.nodeFour, ptr %108, i64 %idxprom90
  %c92 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx91, i32 0, i32 2
  %arrayidx93 = getelementptr inbounds [4 x i8], ptr %c92, i64 0, i64 1
  %110 = load i8, ptr %arrayidx93, align 1
  %conv94 = sext i8 %110 to i32
  %call95 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv94)
  %111 = load ptr, ptr %arraySeven, align 8
  %112 = load i32, ptr %randomIndex, align 4
  %idxprom96 = sext i32 %112 to i64
  %arrayidx97 = getelementptr inbounds %struct.nodeFour, ptr %111, i64 %idxprom96
  %d98 = getelementptr inbounds %struct.nodeFour, ptr %arrayidx97, i32 0, i32 3
  %arrayidx99 = getelementptr inbounds [1 x i32], ptr %d98, i64 0, i64 0
  %113 = load i32, ptr %arrayidx99, align 4
  %call100 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, i32 noundef %113)
  %114 = load ptr, ptr %arraySeven, align 8
  call void @free(ptr noundef %114) #7
  %115 = load ptr, ptr %arrayEight, align 8
  call void @free(ptr noundef %115) #7
  ret i32 0
}

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef) #3

; Function Attrs: nounwind allocsize(0,1)
declare dso_local noalias ptr @calloc(i64 noundef, i64 noundef) #4

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #5

declare dso_local i32 @printf(ptr noundef, ...) #6

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind }
attributes #8 = { nounwind willreturn memory(read) }
attributes #9 = { nounwind allocsize(0,1) }
attributes #10 = { nounwind allocsize(0) }

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
!11 = distinct !{!11, !5}
!12 = distinct !{!12, !5}
!13 = distinct !{!13, !5}
!14 = distinct !{!14, !5}
!15 = distinct !{!15, !5}
!16 = distinct !{!16, !5}
!17 = distinct !{!17, !5}
!18 = distinct !{!18, !5}
!19 = distinct !{!19, !5}
!20 = distinct !{!20, !5}
!21 = distinct !{!21, !5}
!22 = distinct !{!22, !5}
!23 = distinct !{!23, !5}
!24 = distinct !{!24, !5}
!25 = distinct !{!25, !5}
!26 = distinct !{!26, !5}
!27 = distinct !{!27, !5}
